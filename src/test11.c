#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minimp3_ex.h"

#define INPUT_BUFFER_SIZE (4 * 1024)                           // 16KB 输入缓冲区
#define OUTPUT_BUFFER_SIZE (MINIMP3_MAX_SAMPLES_PER_FRAME * 2) // 输出PCM缓冲区
#define MIN_FRAME_SIZE 1441                                    // MP3帧最大尺寸(320kbps@44.1kHz)

// 立体声转单声道函数
void stereo_to_mono(const short* stereo, short* mono, int samples) {
    // 每个立体声样本包含左右两个声道
    for (int i = 0; i < samples; i++) {
        // 计算左右声道的平均值
        int left = stereo[2 * i];
        int right = stereo[2 * i + 1];
        
        // 避免溢出，使用32位中间值
        int mixed = (left + right) / 2;
        
        // 确保在16位范围内
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;
        
        mono[i] = (short)mixed;
    }
}

int test11()
{
    int convert_to_mono = 1;
    const char *input_file = "D:\\wKgBeVGQ9FqAUoWcAK2pOWIssuA553.mp3";
    const char *output_file = "D:\\wKgBeVGQ9FqAUoWcAK2pOWIssuA553.pcm";

    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("Failed to open input file");
        return 1;
    }

    FILE *fout = fopen(output_file, "wb");
    if (!fout) {
        perror("Failed to create output file");
        fclose(fin);
        return 1;
    }

    // 初始化解码器
    mp3dec_t mp3d;
    mp3dec_init(&mp3d);

    // 输入缓冲区
    uint8_t *in_buf = (uint8_t *)malloc(INPUT_BUFFER_SIZE + MIN_FRAME_SIZE);
    if (!in_buf) {
        perror("Memory allocation failed");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    // PCM输出缓冲区
    short pcm[OUTPUT_BUFFER_SIZE];
    short mono_pcm[MINIMP3_MAX_SAMPLES_PER_FRAME]; // 单声道缓冲区
    mp3dec_frame_info_t info;
    memset(&info, 0, sizeof(info));

    size_t bytes_read = 0;     // 当前缓冲区中有效数据量
    size_t bytes_consumed = 0; // 已处理数据量
    size_t total_pcm = 0;      // 总解码PCM样本数
    int sample_rate = 0;       // 采样率
    int channels = 0;          // 声道数
    int output_channels = 0;   // 输出声道数

    printf("Processing: %s\n", input_file);
    printf("Output: %s\n", output_file);
    if (convert_to_mono) {
        printf("Mode: Stereo to Mono conversion\n");
    } else {
        printf("Mode: Original channels\n");
    }

    // 跳过ID3v2标签（如果存在）
    uint8_t id3_header[10];
    if (fread(id3_header, 1, 10, fin) == 10) {
        size_t id3_size = mp3dec_skip_id3v2(id3_header, 10);
        if (id3_size > 0) {
            fseek(fin, id3_size, SEEK_SET);
            printf("Skipped ID3v2 tag (%zu bytes)\n", id3_size);
        } else {
            // 没有ID3标签，重置到文件开头
            fseek(fin, 0, SEEK_SET);
        }
    }

    // 主解码循环
    while (1) {
        // 移动未处理数据到缓冲区前端
        if (bytes_consumed > 0) {
            size_t remaining = bytes_read - bytes_consumed;
            if (remaining > 0) {
                memmove(in_buf, in_buf + bytes_consumed, remaining);
            }
            bytes_read = remaining;
            bytes_consumed = 0;
        }

        // 从文件读取更多数据
        size_t to_read = INPUT_BUFFER_SIZE - bytes_read;
        if (to_read > 0) {
            size_t n = fread(in_buf + bytes_read, 1, to_read, fin);
            if (n == 0 && bytes_read == 0) {
                break; // 文件结束
            }
            bytes_read += n;
        }

        // 解码帧
        int frame_samples = mp3dec_decode_frame(&mp3d, in_buf, bytes_read, pcm, &info);

        if (frame_samples > 0) {
            // 首次解码时记录音频参数
            if (sample_rate == 0) {
                sample_rate = info.hz;
                channels = info.channels;
                output_channels = convert_to_mono ? 1 : channels;

                printf("\nAudio Info:\n");
                printf("  Sample Rate: %d Hz\n", sample_rate);
                printf("  Input Channels: %d\n", channels);
                printf("  Output Channels: %d\n", output_channels);
                printf("  Layer: %d\n", info.layer);
                printf("  Bitrate: %d kbps\n", info.bitrate_kbps);
                printf("  Frame Bytes: %d\n", info.frame_bytes);
            }

            // 立体声转单声道处理
            if (convert_to_mono && channels == 2) {
                // 转换立体声为单声道
                stereo_to_mono(pcm, mono_pcm, frame_samples);

                // 写入单声道PCM数据
                fwrite(mono_pcm, sizeof(short), frame_samples, fout);
                total_pcm += frame_samples;
            } else if (convert_to_mono && channels == 1) {
                // 单声道输入，直接写入（无需转换）
                fwrite(pcm, sizeof(short), frame_samples, fout);
                total_pcm += frame_samples;
            } else {
                // 保持原始声道
                size_t pcm_samples = frame_samples * channels;
                fwrite(pcm, sizeof(short), pcm_samples, fout);
                total_pcm += pcm_samples;
            }

            // 更新已处理字节数
            bytes_consumed = info.frame_bytes;
        } else if (info.frame_bytes > 0) {
            // 找到帧头但数据不足
            bytes_consumed = info.frame_bytes;
        } else {
            // 未找到有效帧头，跳过1字节继续搜索
            bytes_consumed = 1;
        }

        // 进度显示
        if (ftell(fin) % (100 * 1024) == 0) {
            long position = ftell(fin);
            fseek(fin, 0, SEEK_END);
            long total_size = ftell(fin);
            fseek(fin, position, SEEK_SET);

            printf("Decoding progress: %.1f%%\r", (double)position / total_size * 100);
            fflush(stdout);
        }
    }

    // 完成解码
    printf("\nDecoding complete!\n");
    printf("Total PCM samples: %zu\n", total_pcm);
    printf("Duration: %.2f seconds\n", (double)total_pcm / (sample_rate * channels));

    // 清理资源
    free(in_buf);
    fclose(fin);
    fclose(fout);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "minimp3_ex.h"

#define INPUT_BUFFER_SIZE (4 * 1024)                           // 16KB 输入缓冲区
#define OUTPUT_BUFFER_SIZE (MINIMP3_MAX_SAMPLES_PER_FRAME * 2) // 输出PCM缓冲区
#define MIN_FRAME_SIZE 1441                                    // MP3帧最大尺寸(320kbps@44.1kHz)

// 重采样配置
#define RESAMPLE_FIR_SIZE 128 // FIR滤波器大小
#define MAX_RESAMPLE_RATIO 8  // 最大重采样比率

// 重采样上下文结构
typedef struct
{
    int src_rate;    // 源采样率
    int dst_rate;    // 目标采样率
    double ratio;    // 重采样比率 (src/dst)
    double position; // 当前位置（小数）

    // FIR滤波器相关
    float fir_coeff[RESAMPLE_FIR_SIZE];
    float history[RESAMPLE_FIR_SIZE];
    int hist_idx;
} ResamplerContext;

// 初始化重采样器
void init_resampler(ResamplerContext *ctx, int src_rate, int dst_rate)
{
    memset(ctx, 0, sizeof(ResamplerContext));
    ctx->src_rate = src_rate;
    ctx->dst_rate = dst_rate;
    ctx->ratio = (double)src_rate / dst_rate;

    // 设计FIR低通滤波器（防止混叠）
    double cutoff = 0.9 * 0.5 * (dst_rate < src_rate ? dst_rate : src_rate);
    double sum = 0.0;

    for (int i = 0; i < RESAMPLE_FIR_SIZE; i++) {
        double n = i - RESAMPLE_FIR_SIZE / 2;
        double h = 0.0;

        if (n == 0) {
            h = 2 * M_PI * cutoff / src_rate;
        } else {
            h = sin(2 * M_PI * cutoff * n / src_rate) / (M_PI * n);
        }

        // 应用Hamming窗
        double w = 0.54 - 0.46 * cos(2 * M_PI * i / (RESAMPLE_FIR_SIZE - 1));
        ctx->fir_coeff[i] = h * w;
        sum += ctx->fir_coeff[i];
    }

    // 归一化
    for (int i = 0; i < RESAMPLE_FIR_SIZE; i++) {
        ctx->fir_coeff[i] /= sum;
    }
}

// 执行重采样
int resample_audio(ResamplerContext *ctx, const short *input, int in_samples, short *output, int max_out_samples)
{
    if (ctx->src_rate == ctx->dst_rate) {
        // 采样率相同，直接复制
        int copy_count = in_samples < max_out_samples ? in_samples : max_out_samples;
        memcpy(output, input, copy_count * sizeof(short));
        return copy_count;
    }

    int out_idx = 0;

    for (int i = 0; out_idx < max_out_samples && ctx->position < in_samples; out_idx++) {
        // 计算当前采样位置（小数）
        double pos = ctx->position;
        int idx = (int)pos;
        double frac = pos - idx;

        // 更新FIR滤波器历史
        ctx->history[ctx->hist_idx] = input[idx];
        ctx->hist_idx = (ctx->hist_idx + 1) % RESAMPLE_FIR_SIZE;

        // FIR滤波（抗混叠）
        float filtered = 0.0;
        int hist_pos = ctx->hist_idx;

        for (int j = 0; j < RESAMPLE_FIR_SIZE; j++) {
            filtered += ctx->fir_coeff[j] * ctx->history[hist_pos];
            hist_pos = (hist_pos + 1) % RESAMPLE_FIR_SIZE;
        }

        // 线性插值
        float sample;
        if (idx < in_samples - 1) {
            sample = (1.0f - frac) * filtered + frac * input[idx + 1];
        } else {
            sample = filtered;
        }

        // 限幅并存储
        if (sample > 32767.0f)
            sample = 32767.0f;
        if (sample < -32768.0f)
            sample = -32768.0f;
        output[out_idx] = (short)sample;

        // 更新位置
        ctx->position += ctx->ratio;
    }

    // 调整位置为下一帧准备
    ctx->position -= in_samples;

    return out_idx;
}

// 立体声转单声道函数
void stereo_to_mono(const short *stereo, short *mono, int samples)
{
    // 每个立体声样本包含左右两个声道
    for (int i = 0; i < samples; i++) {
        // 计算左右声道的平均值
        int left = stereo[2 * i];
        int right = stereo[2 * i + 1];

        // 避免溢出，使用32位中间值
        int mixed = (left + right) / 2;

        // 确保在16位范围内
        if (mixed > 32767)
            mixed = 32767;
        if (mixed < -32768)
            mixed = -32768;

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
    short mono_pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];                           // 单声道缓冲区
    short resampled_pcm[MINIMP3_MAX_SAMPLES_PER_FRAME * MAX_RESAMPLE_RATIO]; // 重采样PCM

    mp3dec_frame_info_t info;
    memset(&info, 0, sizeof(info));

    size_t bytes_read = 0;        // 当前缓冲区中有效数据量
    size_t bytes_consumed = 0;    // 已处理数据量
    size_t total_pcm_samples = 0; // 总解码PCM样本数
    int sample_rate = 0;          // 采样率
    int channels = 0;             // 声道数
    int output_channels = 0;      // 输出声道数
    int resample_rate = 16000;
    long last_pos = 0;

    printf("Processing: %s\n", input_file);
    printf("Output: %s\n", output_file);
    if (convert_to_mono) {
        printf("Mode: Stereo to Mono conversion\n");
    } else {
        printf("Mode: Original channels\n");
    }

    // 跳过ID3v2标签（如果存在）
    uint8_t id3_header[10];
    size_t id3_size = 0;
    if (fread(id3_header, 1, 10, fin) == 10) {
        id3_size = mp3dec_skip_id3v2(id3_header, 10);
        if (id3_size > 0) {
            printf("Skipped ID3v2 tag (%zu bytes)\n", id3_size);
        } else {
            // 没有ID3标签，重置到文件开头
        }
    }

    // 获取文件大小用于进度显示
    fseek(fin, 0, SEEK_END);
    long total_file_size = ftell(fin);
    fseek(fin, id3_size, SEEK_SET); // 回到跳过ID3后的位置

    // 重采样上下文
    ResamplerContext resampler;
    if (resample_rate > 0) {
        printf("Resampling enabled. Target rate: %d Hz\n", resample_rate);
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

                // 初始化重采样器
                if (resample_rate > 0) {
                    init_resampler(&resampler, sample_rate, resample_rate);
                    sample_rate = resample_rate; // 输出采样率变为目标采样率
                }

                printf("\nAudio Info:\n");
                printf("  Sample Rate: %d Hz\n", sample_rate);
                printf("  Input Channels: %d\n", channels);
                printf("  Output Channels: %d\n", output_channels);
                printf("  Layer: %d\n", info.layer);
                printf("  Bitrate: %d kbps\n", info.bitrate_kbps);
                printf("  Frame Bytes: %d\n", info.frame_bytes);
                if (resample_rate > 0) {
                    printf("  Resampling: %d Hz -> %d Hz\n", info.hz, resample_rate);
                }
            }

            short *output_buffer = pcm;
            int samples_to_write = frame_samples * output_channels;

            // 声道转换
            if (convert_to_mono && channels == 2) {
                stereo_to_mono(pcm, mono_pcm, frame_samples);
                output_buffer = mono_pcm;
                samples_to_write = frame_samples; // 单声道样本数
            }

            // 重采样处理
            if (resample_rate > 0) {
                int resampled_count = resample_audio(&resampler, output_buffer,
                    convert_to_mono ? frame_samples : frame_samples * channels, resampled_pcm,
                    sizeof(resampled_pcm) / sizeof(short));

                output_buffer = resampled_pcm;
                samples_to_write = resampled_count;
            }

            // 写入PCM数据
            fwrite(output_buffer, sizeof(short), samples_to_write, fout);
            total_pcm_samples += samples_to_write;

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
        long current_pos = ftell(fin) - (long)bytes_read + (long)bytes_consumed;
        // printf("current_pos: %ld, total_file_size: %ld\n", current_pos, total_file_size);
        if (current_pos - last_pos > 10 * 1024) {
            float progress = (float)current_pos / total_file_size * 100;
            printf("Decoding progress: %.1f%%\r", progress);
            fflush(stdout);
            last_pos = current_pos;
        } else if (total_file_size <= 10 * 1024) {
            float progress = (float)current_pos / total_file_size * 100;
            printf("Decoding progress: %.1f%%\r", progress);
            fflush(stdout);
        }

        // 检查是否到达文件末尾
        if (feof(fin) && bytes_consumed >= bytes_read) {
            break;
        }
    }

    // 完成解码
    printf("\nDecoding complete!\n");
    printf("Total PCM samples: %zu\n", total_pcm_samples);
    // 计算时长
    double duration = (double)total_pcm_samples / (sample_rate * output_channels);
    int minutes = (int)(duration / 60);
    double seconds = duration - minutes * 60;
    printf("Duration: %d minutes %.2f seconds\n", minutes, seconds);

    // 清理资源
    free(in_buf);
    fclose(fin);
    fclose(fout);

    return 0;
}
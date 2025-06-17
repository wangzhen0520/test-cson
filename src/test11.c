#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "minimp3.h"

// 重采样上下文结构
typedef struct {
    int src_rate;       // 源采样率
    int dst_rate;       // 目标采样率
    double ratio;       // 重采样比率 (src/dst)
    double position;    // 当前位置（小数）
    bool initialized;   // 是否已初始化
} ResamplerContext;

// 初始化重采样器
void init_resampler(ResamplerContext *ctx, int src_rate, int dst_rate) {
    if (src_rate <= 0 || dst_rate <= 0) {
        fprintf(stderr, "Invalid sample rate: src=%d, dst=%d\n", src_rate, dst_rate);
        exit(1);
    }
    
    memset(ctx, 0, sizeof(ResamplerContext));
    ctx->src_rate = src_rate;
    ctx->dst_rate = dst_rate;
    ctx->ratio = (double)src_rate / dst_rate;
    ctx->initialized = true;
}

// 安全的线性插值重采样
int resample_audio(ResamplerContext *ctx, const short *input, int in_samples, 
                   short *output, int max_out_samples, int channels) {
    // 参数有效性检查
    if (!input || !output || !ctx->initialized || in_samples <= 0 || 
        max_out_samples <= 0 || channels <= 0) {
        fprintf(stderr, "Invalid resampling parameters\n");
        return 0;
    }
    
    // 采样率相同，直接复制
    if (ctx->src_rate == ctx->dst_rate) {
        int copy_count = in_samples < max_out_samples ? in_samples : max_out_samples;
        memcpy(output, input, copy_count * sizeof(short) * channels);
        return copy_count;
    }
    
    int out_idx = 0;
    double step = 1.0 / ctx->ratio;
    
    // 计算最大可能的输出样本数
    int max_possible_samples = (int)(in_samples / ctx->ratio) + 1;
    if (max_possible_samples > max_out_samples) {
        max_possible_samples = max_out_samples;
    }
    
    // 确保位置在合理范围内
    if (ctx->position < 0) ctx->position = 0;
    if (ctx->position >= in_samples) ctx->position = in_samples - 1;
    
    while (out_idx < max_possible_samples && ctx->position < in_samples) {
        int idx1 = (int)ctx->position;        // 前一个样本索引
        int idx2 = idx1 + 1;                  // 后一个样本索引
        
        // 确保索引在有效范围内
        if (idx1 >= in_samples) idx1 = in_samples - 1;
        if (idx2 >= in_samples) idx2 = in_samples - 1;
        if (idx1 < 0) idx1 = 0;
        if (idx2 < 0) idx2 = 0;
        
        double frac = ctx->position - idx1;   // 小数部分
        
        for (int c = 0; c < channels; c++) {
            // 安全获取样本
            int ch_offset1 = idx1 * channels + c;
            int ch_offset2 = idx2 * channels + c;
            
            if (ch_offset1 >= in_samples * channels) 
                ch_offset1 = in_samples * channels - 1;
            if (ch_offset2 >= in_samples * channels) 
                ch_offset2 = in_samples * channels - 1;
            if (ch_offset1 < 0) ch_offset1 = 0;
            if (ch_offset2 < 0) ch_offset2 = 0;
            
            short s1 = input[ch_offset1];
            short s2 = input[ch_offset2];
            
            // 线性插值
            double sample = s1 + (s2 - s1) * frac;
            
            // 限幅处理
            if (sample > 32767.0) sample = 32767.0;
            if (sample < -32768.0) sample = -32768.0;
            
            output[out_idx * channels + c] = (short)sample;
        }
        
        out_idx++;
        ctx->position += step;
    }
    
    // 为下一帧准备位置
    ctx->position -= in_samples;
    
    return out_idx;
}

// 立体声转单声道
void stereo_to_mono(const short* stereo, short* mono, int samples) {
    if (!stereo || !mono || samples <= 0) return;
    
    for (int i = 0; i < samples; i++) {
        // 确保索引在有效范围内
        int idx1 = 2 * i;
        int idx2 = 2 * i + 1;
        
        if (idx1 < 0) idx1 = 0;
        if (idx2 < 0) idx2 = 0;
        if (idx1 >= samples * 2) idx1 = samples * 2 - 1;
        if (idx2 >= samples * 2) idx2 = samples * 2 - 1;
        
        int left = stereo[idx1];
        int right = stereo[idx2];
        int mixed = (left + right) / 2;
        
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;
        
        mono[i] = (short)mixed;
    }
}

// 安全的ID3标签跳过函数
size_t skip_id3_tags(FILE *file) {
    if (!file) return 0;
    
    uint8_t header[10];
    size_t bytes_skipped = 0;
    
    if (fread(header, 1, 10, file) == 10) {
        if (header[0] == 'I' && header[1] == 'D' && header[2] == '3') {
            size_t tag_size = ((header[6] & 0x7F) << 21) |
                             ((header[7] & 0x7F) << 14) |
                             ((header[8] & 0x7F) << 7) |
                             (header[9] & 0x7F);
            bytes_skipped = 10 + tag_size;
            
            // 安全跳过标签
            if (fseek(file, tag_size, SEEK_CUR) != 0) {
                perror("Error skipping ID3 tag");
            }
        } else {
            // 不是ID3标签，回到文件开头
            fseek(file, 0, SEEK_SET);
        }
    } else {
        fseek(file, 0, SEEK_SET);
    }
    return bytes_skipped;
}

int main_entry(int argc, char *argv[]) {
    // 参数解析
    if (argc < 4) {
        printf("Usage: %s [-mono] [-resample TARGET_RATE] <input.mp3> <output.pcm>\n", argv[0]);
        printf("Options:\n");
        printf("  -mono          Convert stereo to mono\n");
        printf("  -resample RATE Resample to target rate (e.g., 16000)\n");
        return 1;
    }

    int convert_to_mono = 0;
    int resample_rate = 0;
    const char *input_file = NULL;
    const char *output_file = NULL;
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-mono") == 0) {
            convert_to_mono = 1;
        } else if (strcmp(argv[i], "-resample") == 0 && i + 1 < argc) {
            resample_rate = atoi(argv[++i]);
            if (resample_rate <= 0) {
                fprintf(stderr, "Invalid resample rate: %d\n", resample_rate);
                return 1;
            }
        } else {
            if (!input_file) {
                input_file = argv[i];
            } else {
                output_file = argv[i];
            }
        }
    }

    if (!input_file || !output_file) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    
    // 打开文件
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

    // 跳过ID3标签
    size_t id3_size = skip_id3_tags(fin);
    if (id3_size > 0) {
        printf("Skipped ID3 tag (%zu bytes)\n", id3_size);
    }

    // 初始化解码器
    mp3dec_t mp3d;
    mp3dec_init(&mp3d);
    
    // 输入缓冲区
    const size_t INPUT_BUFFER_SIZE = 24 * 1024;  // 24KB
    const size_t MIN_FRAME_SIZE = 1441;          // MP3帧最大尺寸
    uint8_t *in_buf = (uint8_t*)malloc(INPUT_BUFFER_SIZE + MIN_FRAME_SIZE);
    if (!in_buf) {
        perror("Memory allocation failed");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    
    // PCM缓冲区
    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME * 2];      // 原始PCM（最大2声道）
    short mono_pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];     // 单声道PCM
    
    // 重采样缓冲区（动态分配，避免栈溢出）
    short *resampled_pcm = NULL;
    size_t resampled_buffer_size = 0;
    
    mp3dec_frame_info_t info;
    memset(&info, 0, sizeof(info));
    
    size_t bytes_read = 0;
    size_t bytes_consumed = 0;
    size_t total_pcm_samples = 0;
    int sample_rate = 0;
    int channels = 0;
    int output_channels = 0;
    int output_sample_rate = 0;
    
    // 重采样上下文
    ResamplerContext resampler = {0};
    if (resample_rate > 0) {
        printf("Resampling enabled. Target rate: %d Hz\n", resample_rate);
    }
    
    // 获取文件大小
    fseek(fin, 0, SEEK_END);
    long total_file_size = ftell(fin);
    fseek(fin, id3_size, SEEK_SET);
    
    printf("Processing: %s\n", input_file);
    printf("Output: %s\n", output_file);
    if (convert_to_mono) printf("Mode: Stereo to Mono\n");
    
    // 主解码循环
    while (!feof(fin)) {
        // 移动缓冲区数据
        if (bytes_consumed > 0) {
            size_t remaining = bytes_read - bytes_consumed;
            if (remaining > 0) {
                memmove(in_buf, in_buf + bytes_consumed, remaining);
            }
            bytes_read = remaining;
            bytes_consumed = 0;
        }
        
        // 读取更多数据
        size_t to_read = INPUT_BUFFER_SIZE - bytes_read;
        if (to_read > 0) {
            size_t n = fread(in_buf + bytes_read, 1, to_read, fin);
            if (n == 0 && bytes_read == 0) break;
            bytes_read += n;
        }
        
        // 解码帧
        int frame_samples = mp3dec_decode_frame(&mp3d, in_buf, bytes_read, pcm, &info);
        
        if (frame_samples > 0) {
            // 首次解码时初始化参数
            if (sample_rate == 0) {
                if (info.hz <= 0) {
                    fprintf(stderr, "Invalid sample rate: %d Hz\n", info.hz);
                    break;
                }
                
                sample_rate = info.hz;
                channels = info.channels;
                output_channels = convert_to_mono ? 1 : channels;
                output_sample_rate = resample_rate > 0 ? resample_rate : sample_rate;
                
                // 初始化重采样器
                if (resample_rate > 0) {
                    init_resampler(&resampler, sample_rate, resample_rate);
                    
                    // 分配重采样缓冲区（足够大以容纳重采样后的数据）
                    resampled_buffer_size = (size_t)(frame_samples * resample_rate / sample_rate + 10) * output_channels;
                    resampled_pcm = (short*)malloc(resampled_buffer_size * sizeof(short));
                    if (!resampled_pcm) {
                        perror("Failed to allocate resampling buffer");
                        break;
                    }
                }
                
                printf("\nAudio Info:\n");
                printf("  Sample Rate: %d Hz\n", sample_rate);
                printf("  Input Channels: %d\n", channels);
                printf("  Output Channels: %d\n", output_channels);
                if (resample_rate > 0) {
                    printf("  Resampling: %d Hz -> %d Hz\n", sample_rate, resample_rate);
                }
                printf("  Bitrate: %d kbps\n", info.bitrate_kbps);
            }
            
            short *output_buffer = pcm;
            int samples_to_write = frame_samples * output_channels;
            int processed_samples = frame_samples;
            
            // 声道转换
            if (convert_to_mono && channels == 2) {
                stereo_to_mono(pcm, mono_pcm, frame_samples);
                output_buffer = mono_pcm;
                samples_to_write = frame_samples; // 单声道样本数
                processed_samples = frame_samples;
            } else if (convert_to_mono && channels == 1) {
                // 单声道直接使用
                output_buffer = pcm;
                samples_to_write = frame_samples;
                processed_samples = frame_samples;
            }
            
            // 重采样处理
            if (resample_rate > 0 && resampled_pcm) {
                int max_out_samples = (int)resampled_buffer_size / output_channels;
                int resampled_count = resample_audio(
                    &resampler, 
                    output_buffer, 
                    processed_samples, 
                    resampled_pcm, 
                    max_out_samples,
                    output_channels
                );
                
                if (resampled_count > 0) {
                    output_buffer = resampled_pcm;
                    samples_to_write = resampled_count * output_channels;
                } else {
                    fprintf(stderr, "Resampling failed for frame\n");
                }
            }
            
            // 写入PCM数据
            if (samples_to_write > 0) {
                fwrite(output_buffer, sizeof(short), samples_to_write, fout);
                total_pcm_samples += samples_to_write;
            }
            
            bytes_consumed = info.frame_bytes;
        } 
        else if (info.frame_bytes > 0) {
            bytes_consumed = info.frame_bytes;
        }
        else {
            bytes_consumed = 1;
        }
        
        // 进度显示
        long current_pos = ftell(fin) - (long)bytes_read + (long)bytes_consumed;
        if (current_pos % (500 * 1024) == 0) {
            float progress = (float)current_pos / total_file_size * 100;
            printf("Decoding: %.1f%%\r", progress);
            fflush(stdout);
        }
        
        // 文件结束检查
        if (feof(fin) && bytes_consumed >= bytes_read) {
            break;
        }
    }
    
    // 完成解码
    printf("\n\nDecoding complete!\n");
    printf("Total PCM samples: %zu\n", total_pcm_samples);
    
    if (output_sample_rate > 0 && output_channels > 0) {
        // 计算时长
        double duration = (double)total_pcm_samples / (output_sample_rate * output_channels);
        int minutes = (int)(duration / 60);
        double seconds = duration - minutes * 60;
        printf("Duration: %d minutes %.2f seconds\n", minutes, seconds);
    }
    
    // 文件大小信息
    double original_size_mb = (double)total_file_size / (1024 * 1024);
    double pcm_size_mb = (double)total_pcm_samples * sizeof(short) / (1024 * 1024);
    printf("Original MP3 size: %.2f MB\n", original_size_mb);
    printf("PCM output size: %.2f MB\n", pcm_size_mb);
    
    // 清理资源
    free(in_buf);
    if (resampled_pcm) free(resampled_pcm);
    fclose(fin);
    fclose(fout);
    
    return 0;
}

int test11() {
    char *argv[] = {"", "-mono", "-resample", "16000", "D:\\wKgBeVGQ9FqAUoWcAK2pOWIssuA553.mp3", "D:\\wKgBeVGQ9FqAUoWcAK2pOWIssuA553.pcm"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    main_entry(argc, argv);
}
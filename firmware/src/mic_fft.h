/**
 * @file mic_fft.h
 * @brief 麦克风采集 + FFT 频谱计算
 */

#ifndef LIGHTFRAME_MIC_FFT_H
#define LIGHTFRAME_MIC_FFT_H

#include "config.h"

// FFT 结果输出：每个频段的幅值
extern float fft_bands[FFT_BANDS];

void mic_fft_init();
void mic_fft_sample_and_analyze();
void mic_fft_get_bands(float *out_bands);

#endif // LIGHTFRAME_MIC_FFT_H

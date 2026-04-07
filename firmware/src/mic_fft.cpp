/**
 * @file mic_fft.cpp
 * @brief 麦克风采集 + FFT 频谱计算实现
 */

#include <ArduinoFFT.h>
#include "mic_fft.h"

float fft_bands[FFT_BANDS];

static double v_real[SAMPLES];
static double v_imag[SAMPLES];
static ArduinoFFT<double> fft = ArduinoFFT<double>(v_real, v_imag, SAMPLES, SAMPLE_FREQ);

void mic_fft_init() {
    analogReadResolution(12);

    // 初始化频带数组
    for (int i = 0; i < FFT_BANDS; i++) {
        fft_bands[i] = 0;
    }
}

void mic_fft_sample_and_analyze() {
    // 采样
    for (int i = 0; i < SAMPLES; i++) {
        int raw = analogRead(MIC_PIN);
        v_real[i] = (double)raw - 2048;  // 偏移到零点
        v_imag[i] = 0;
        delayMicroseconds(1000000 / SAMPLE_FREQ);
    }

    // 窗函数
    fft.windowing(FFT_WIN_TYP_HANN);
    fft.compute();
    fft.complexToMagnitude();

    // 计算每个频段的能量
    int band_size = (SAMPLES / 2) / FFT_BANDS;

    for (int band = 0; band < FFT_BANDS; band++) {
        float sum = 0;
        int start = band * band_size + 1;
        int end = (band + 1) * band_size;

        for (int i = start; i <= end; i++) {
            sum += (float)v_real[i];
        }

        fft_bands[band] = sum / band_size / 100.0;
        if (fft_bands[band] < 0.5) fft_bands[band] = 0;  // 噪声门
    }
}

void mic_fft_get_bands(float *out_bands) {
    for (int i = 0; i < FFT_BANDS; i++) {
        out_bands[i] = fft_bands[i];
    }
}

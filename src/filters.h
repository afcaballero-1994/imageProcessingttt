#ifndef IMAGEPROCESSINGTEST_FILTERS_H
#define IMAGEPROCESSINGTEST_FILTERS_H
#include "common.h"
#include <math.h>

typedef struct matrix{
    u8 rows;
    u8 cols;
    f32 data[32 * 32];
} Matrix;

void generateGaussKernel(u8 numColsRows, f32 sigma, Matrix *kernel);
void printKernel(const Matrix *kernel);

i64 acpMod(i64 a, i64 b);
f32 clamp(f32 value);
static float sRGBtoLin(u8 color);
static float getLuminance(u8 r, u8 g, u8 b);
static float getPerceivedL(f32 luminance);
void processImagePerceivedLuminance(u8 *data, u32 width, u32 height, u32 channels);
void processImageSepiaTone(u8 *data, u32 width, u32 height, u32 channels);
void gaussianBlur(u8 *data,i64 width, i64 height, i64 channels ,u8 kernelSize, f32 sigma);

#endif //IMAGEPROCESSINGTEST_FILTERS_H

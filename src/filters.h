#ifndef IMAGEPROCESSINGTEST_FILTERS_H
#define IMAGEPROCESSINGTEST_FILTERS_H
#include "common.h"
#include <math.h>

f32 clamp(f32 value);
static float sRGBtoLin(u8 color);
static float getLuminance(u8 r, u8 g, u8 b);
static float getPerceivedL(f32 luminance);
void processImagePerceivedLuminance(u8 *data, u32 width, u32 height, u32 channels);
void processImageSepiaTone(u8 *data, u32 width, u32 height, u32 channels);

#endif //IMAGEPROCESSINGTEST_FILTERS_H

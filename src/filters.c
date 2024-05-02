#include "filters.h"

static float sRGBtoLin(const u8 color) {
    float vc = (float) color / 255.0f;
    if (vc <= 0.04045) {
        return vc / 12.92f;
    } else {
        return powf(((vc + 0.055f) / 1.055f), 2.4f);
    }
}

static float getLuminance(const u8 r, const u8 g, const u8 b) {
    return (sRGBtoLin(r) * 0.2126f) + (sRGBtoLin(g) * 0.7152f) +
           (sRGBtoLin(b) * 0.0722f);
}

static float getPerceivedL(const f32 luminance) {
    if (luminance <= (216.0f / 24389.0f)) {
        return luminance * (24389.0f / 27.0f);
    } else {
        return powf(luminance, (1.0f / 3.0f)) * 116.0f - 16.0f;
    }
}

void processImagePerceivedLuminance(u8 *data, u32 width, u32 height, u32 channels){

    for (u64 i = 0; i < width * height * channels; i += channels) {

        int r = data[i];
        int g = data[i + 1];
        int b = data[i + 2];

        float luminance = getLuminance(r, g, b);
        float perceivedLuminance = getPerceivedL(luminance);

        data[i] = perceivedLuminance * 2.55999;
        data[i + 1] = perceivedLuminance * 2.55999;
        data[i + 2] = perceivedLuminance * 2.55999;
    }
}
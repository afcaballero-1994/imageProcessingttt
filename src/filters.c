#include "filters.h"
#include "stb_image.h"

inline u8 clamp(u8 value, u8 limit){
    if(value > limit){
        return limit;
    } else if(value < 0){
        return 0;
    } else{
        return value;
    }
}

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

void processImageSepiaTone(u8 *data, u32 width, u32 height, u32 channels){
    for (u64 i = 0; i < width * height * channels; i += channels) {

        //tr = 0.393R + 0.769G + 0.189B
        //tg = 0.349R + 0.686G + 0.168B
        //tb = 0.272R + 0.534G + 0.131B
        u8 r = data[i];
        u8 g = data[i + 1];
        u8 b = data[i + 2];


        f32 tr = (0.393f * (f32) r) + (0.769f * (f32) g) + (0.189f * (f32) b);
        f32 tg = (0.349f * (f32) r) + (0.686f * (f32) g) + (0.168f * (f32) b);
        f32 tb = (0.272f * (f32) r) + (0.534f * (f32) g) + (0.131f * (f32) b);

        data[i    ] = clamp((u8)tr, 255);
        data[i + 1] = clamp((u8)tg, 255);
        data[i + 2] = clamp((u8)tb, 255);
    }
}
#include <assert.h>
#include "filters.h"
#include "stb_image.h"
#include <string.h>

i64 acpMod(i64 a, i64 b){
    i64 mod = a % b;
    if(mod < 0){
        return mod + b;
    } else{
        return mod;
    }
}

f32 clamp(f32 value){
    if(value > 255){
        return 255;
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

        data[i    ] = clamp(tr);
        data[i + 1] = clamp(tg);
        data[i + 2] = clamp(tb);
    }
}

void generateGaussKernel(const u8 numColsRows, const f32 sigma ,Matrix *kernel){
    assert(numColsRows < 32 && "Number of columns and rows cannot be larger than 32");
    const f32 pi = atan2f(0.0f,-1.0f);
    const f32 fraction = 1.0f / (2 * pi * sigma * sigma);
    kernel->cols = numColsRows, kernel->rows = numColsRows;
    f32 sum = 0;
    for(u8 i = 0; i < numColsRows; i++){
        for(u8 j = 0; j < numColsRows; j++){
            f32 x = i - (numColsRows - 1) / 2.0;
            f32 y = j - (numColsRows - 1) / 2.0;
            f32 xtemp = -((x * x) + (y * y)) / (2 * sigma * sigma);
            f32 ex = fraction * expf(xtemp);
            kernel->data[i * numColsRows + j] = ex;
            sum += kernel->data[i * numColsRows + j];
        }
    }
    for(u8 i = 0; i < numColsRows; i++){
        for(u8 j = 0; j < numColsRows; j++){
            kernel->data[i * numColsRows + j] = kernel->data[i * numColsRows + j] / sum;
        }
    }
}

void printKernel(Matrix const *kernel){
    f32 sum = 0.0f;
    for(u8 i = 0; i < kernel->rows; i++){
        for(u8 j = 0; j < kernel->cols; j++){
            printf("%f ", kernel->data[i * kernel->cols + j]);
            sum += kernel->data[i * kernel->cols + j];
        }
        printf("\n");
    }
    fprintf(stdout, "%f", sum);
}

void gaussianBlur(u8 *data,i64 width, i64 height, i64 channels ,u8 kernelSize, f32 sigma){
    Matrix kernel;
    generateGaussKernel(kernelSize, sigma, &kernel);
    u8 center = kernelSize / 2.0f;
    u8 *temp = malloc(width * height * channels);

    for(i64 i = 0; i < width; i++){
        for(i64 j = 0; j < height; j++){
            f32 sumRed = 0;
            f32 sumGreen = 0;
            f32 sumBlue = 0;

            for(u8 x = 0; x < kernel.rows; x++){
                for(u8 y = 0; y < kernel.cols; y++){
                    i64 px = acpMod(i + (x - center), width);
                    i64 py = acpMod(j + (y - center), height);

                    sumRed   += (f32) data[channels * (px * height + py)    ] * kernel.data[x * kernelSize + y];
                    sumGreen += (f32) data[channels * (px * height + py) + 1] * kernel.data[x * kernelSize + y];
                    sumBlue  += (f32) data[channels * (px * height + py) + 2] * kernel.data[x * kernelSize + y];
                }
            }

            temp[channels * (i * height + j)    ] = sumRed;
            temp[channels * (i * height + j) + 1] = sumGreen;
            temp[channels * (i * height + j) + 2] = sumBlue;

        }
    }
    memcpy(data, temp, width * height * channels);
    free(temp);
}
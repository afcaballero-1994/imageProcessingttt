#include <stdio.h>
#include "filters.h"
#include "common.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

int main(int argc, char **argv) {

    if (argc > 4 || argc < 3) {
        fprintf(stderr, "Incorrect number of arguments\n");
        exit(1);
    }
    u8 *filename = argv[1];
    u8 *output = argv[2];
    printf("%s\n", filename);
    printf("%s\n", output);

    i32 width, height, channels;
    u8 *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        printf("could not load file\n");
        exit(2);
    }
//    for (size_t i = 0; i < x; i++) {
//        for (size_t j = 0; j < y; j++) {
//            int r = data[n * (j * x + i)];
//            int g = data[n * (j * x + i) + 1];
//            int b = data[n * (j * x + i) + 2];
//
//            data[n * (j * x + i)] =
//                    stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
//            data[n * (j * x + i) + 1] =
//                    stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
//            data[n * (j * x + i) + 2] =
//                    stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
//        }
//    }

    //processImagePerceivedLuminance(data, width, height, channels);
    processImageSepiaTone(data, width, height, channels);

    stbi_write_png(output, width, height, channels, data, channels * width);

    stbi_image_free(data);

    return 0;
}

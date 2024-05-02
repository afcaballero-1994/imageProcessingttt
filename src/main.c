#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#include <math.h>

float sRGBtoLin(unsigned char color) {
  float vc = (float)color / 255.0f;
  if (vc <= 0.04045) {
    return vc / 12.92f;
  } else {
    return powf(((vc + 0.055f) / 1.055f), 2.4f);
  }
}

float getLuminance(unsigned char r, unsigned char g, unsigned char b) {
  return (sRGBtoLin(r) * 0.2126f) + (sRGBtoLin(g) * 0.7152f) +
         (sRGBtoLin(b) * 0.0722f);
}

float getPerceivedL(float luminance) {
  if (luminance <= (216.0f / 24389.0f)) {
    return luminance * (24389.0f / 27.0f);
  } else {
    return powf(luminance, (1.0f / 3.0f)) * 116.0f - 16.0f;
  }
}

int main(void) {
  const char *const filename = "";
  const char *const output = "";
  int x, y, n;
  unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
  if (data == nullptr) {
    printf("could not load file\n");
    exit(2);
  }
  printf("%d %d %d\n", x, y, n);

  //    for (size_t i = 0; i < x * y * n; i += n) {
  //
  //        int r = data[i];
  //        int g = data[i + 1];
  //        int b = data[i + 2];
  //
  //        data[i] = stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
  //        data[i + 1] = stbi__clamp(getPerceivedL(getLuminance(r, g, b))
  //        * 2.55f); data[i + 2] = stbi__clamp(getPerceivedL(getLuminance(r, g,
  //        b)) * 2.55f);
  //    }

  for (size_t i = 0; i < x; i++) {
    for (size_t j = 0; j < y; j++) {
      int r = data[n * (j * x + i)];
      int g = data[n * (j * x + i) + 1];
      int b = data[n * (j * x + i) + 2];

      data[n * (j * x + i)] =
          stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
      data[n * (j * x + i) + 1] =
          stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
      data[n * (j * x + i) + 2] =
          stbi__clamp(getPerceivedL(getLuminance(r, g, b)) * 2.55f);
    }
  }

  stbi_write_png(output, x, y, n, data, n * x);

  stbi_image_free(data);

  return 0;
}

#include <blit/rop2.h>

#include <stdio.h>
#include <assert.h>

int tests_pat() {
  blit_scanline_t vPatBits[] = {
      0x40U, // #. (black-white)
      0x80U, // .# (white-black)
  };
  struct blit_scan imagePat = {
      .store = vPatBits,
      .width = 2,
      .height = 2,
      .stride = 1,
  };
  blit_scanline_t image_bits[8];
  struct blit_scan image = {
      .store = image_bits,
      .width = 8,
      .height = 8,
      .stride = 1,
  };

  for (int y = 0; y < image.height; y += imagePat.height) {
    for (int x = 0; x < image.width; x += imagePat.width) {
      struct blit_rgn1 x_rgn1 = {
          .origin = x,
          .extent = imagePat.width,
          .origin_source = 0,
      };
      struct blit_rgn1 y_rgn1 = {
          .origin = y,
          .extent = imagePat.height,
          .origin_source = 0,
      };
      assert(blit_rop2(&image, &x_rgn1, &y_rgn1, &imagePat, blit_rop2_source_copy));
    }
  }

  for (int x = 0; x < image.width; ++x) {
    for (int y = 0; y < image.height; ++y) {
      blit_scanline_t v[] = {0x00U};
      struct blit_scan imageBit = {
          .store = v,
          .width = 1,
          .height = 1,
          .stride = 1,
      };
      struct blit_rgn1 x_rgn1 = {
          .origin = 0,
          .extent = 1,
          .origin_source = x,
      };
      struct blit_rgn1 y_rgn1 = {
          .origin = 0,
          .extent = 1,
          .origin_source = y,
      };
      assert(blit_rop2(&imageBit, &x_rgn1, &y_rgn1, &image, blit_rop2_source_copy));
      blit_scanline_t bit = v[0] >> 7;
      (void)printf("%c", bit ? '#' : '.');
      assert(bit == ((x & 1U) ^ (y & 1U)));
    }
    (void)printf("\n");
  }
  return 0;
}

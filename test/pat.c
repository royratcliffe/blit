#include <blit/rop2.h>

#include <assert.h>
#include <stdio.h>

int test_pat() {
  blit_scanline_t pat_store[] = {
      0x40U, // #. (black-white)
      0x80U, // .# (white-black)
  };
  struct blit_scan pat = {
      .store = pat_store,
      .width = 2,
      .height = 2,
      .stride = 1,
  };
  blit_scanline_t image_store[8];
  struct blit_scan image = {
      .store = image_store,
      .width = 8,
      .height = 8,
      .stride = 1,
  };

  for (int y = 0; y < image.height; y += pat.height) {
    for (int x = 0; x < image.width; x += pat.width) {
      struct blit_rgn1 x_rgn1 = {
          .origin = x,
          .extent = pat.width,
          .origin_source = 0,
      };
      struct blit_rgn1 y_rgn1 = {
          .origin = y,
          .extent = pat.height,
          .origin_source = 0,
      };
      assert(blit_rop2(&image, &x_rgn1, &y_rgn1, &pat, blit_rop2_copy));
    }
  }

  for (int x = 0; x < image.width; ++x) {
    for (int y = 0; y < image.height; ++y) {
      blit_scanline_t bit_store[] = {0x00U};
      struct blit_scan bit = {
          .store = bit_store,
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
      assert(blit_rop2(&bit, &x_rgn1, &y_rgn1, &image, blit_rop2_copy));
      blit_scanline_t bit_scanline = bit_store[0] >> 7;
      (void)printf("%c", bit_scanline ? '#' : '.');
      assert(bit_scanline == ((x & 1U) ^ (y & 1U)));
    }
    (void)printf("\n");
  }
  return 0;
}

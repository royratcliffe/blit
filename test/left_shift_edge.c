#include <blit/rop2.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_left_shift_edge() {
  BLIT_SCAN_DEFINE_STATIC(result, 80, 80);
  BLIT_SCAN_DEFINE_STATIC(source, 80, 80);

  for (int x = 0; x < result.width; x++) {
    (void)memset(result.store, 0x00U, result.stride * result.height);
    (void)memset(source.store, 0x00U, source.stride * source.height);

    assert(blit_rop2(&source, x, 0, 1, 80, &source, 0, 0, blit_rop2_1));
    assert(blit_rop2(&result, 79, 0, 1, 80, &source, x, 0, blit_rop2_copy));

    for (int y = 0; y < result.height; y++) {
      static const blit_scanline_t expected[] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U};
      assert(memcmp(expected, &result.store[y * result.stride], sizeof(expected)) == 0);
    }
  }

  return EXIT_SUCCESS;
}

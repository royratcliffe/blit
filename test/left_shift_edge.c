#include <blit/rop2.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_left_shift_edge() {
  BLIT_SCAN_DEFINE_STATIC(result, 80, 80);
  BLIT_SCAN_DEFINE_STATIC(source, 80, 80);

  for (int x = 0; x < result.width; x++) {
    /*
     * Clear both the result and source scans to ensure a known starting state
     * at each iteration.
     */
    (void)memset(result.store, 0x00U, result.stride * result.height);
    (void)memset(source.store, 0x00U, source.stride * source.height);

    /*
     * Set a single bit in the source at position (x, 0). This bit will be
     * shifted left to the edge (bit 79) in the result.
     */
    assert(blit_rop2(&source, x, 0, 1, 80, &source, 0, 0, blit_rop2_1));
    assert(blit_rop2(&result, 79, 0, 1, 80, &source, x, 0, blit_rop2_copy));

    /*
     * Verify that the bit has been correctly shifted to the edge in the result
     * scan. Compare each scanline in the result to the hardcoded expectation.
     */
    for (int y = 0; y < result.height; y++) {
      static const blit_scanline_t expected[] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U};
      assert(memcmp(expected, blit_scan_find(&result, 0, y), sizeof(expected)) == 0);
    }
  }

  return EXIT_SUCCESS;
}

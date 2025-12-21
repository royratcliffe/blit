#ifndef __BLIT_SCAN_H__
#define __BLIT_SCAN_H__

#include <stdint.h>

typedef uint8_t blit_scanline_t;

struct blit_scan {
  blit_scanline_t *store;
  int width;
  int height;
  /*!
   * \brief Stride of the scanline buffer.
   * \details The number of bytes between the start of each row in the scanline
   * buffer. The number must be positive (hence the unsigned size type) and at
   * least \c width bytes.
   */
  size_t stride;
};

static inline blit_scanline_t *blit_scan_find(const struct blit_scan *scan, int x, int y) { return scan->store + scan->stride * y + (x >> 3); }

#endif /* __BLIT_SCAN_H__ */

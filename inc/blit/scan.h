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

#endif /* __BLIT_SCAN_H__ */

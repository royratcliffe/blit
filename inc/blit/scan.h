#ifndef __BLIT_SCAN_H__
#define __BLIT_SCAN_H__

#include <stdint.h>

typedef uint8_t blit_scanline_t;

struct blit_scan {
  blit_scanline_t *store;
  int width;
  int height;
  int stride;
};

#endif /* __BLIT_SCAN_H__ */

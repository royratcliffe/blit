/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

#include <blit/phase_align.h>
#include <blit/rop2.h>

/*!
 * \brief 8-bit source operand.
 */
#define S (fetch)

/*!
 * \brief 8-bit destination operand.
 */
#define D (store)

typedef blit_scanline_t (*blit_rop2_func_t)(blit_scanline_t fetch, blit_scanline_t store);

/*!
 * \brief Macro to define a raster operation function.
 * \details The macro ROP_REV_POLISH is used to define the raster operation
 * functions below. Each function takes a fetch function to get the source
 * operand (S) and a pointer to the destination operand (D). The operations are
 * defined using standard bitwise operations.
 *
 * The defined raster operations include:
 *
 * - 0: Always returns 0.
 * - DSon: NOT (D OR S).
 * - DSna: D AND NOT S.
 * - Sn: NOT S.
 * - SDna: S AND NOT D.
 * - Dn: NOT D.
 * - DSx: D XOR S.
 * - DSan: NOT (D AND S).
 * - DSa: D AND S.
 * - DSxn: NOT (D XOR S).
 * - D: D.
 * - DSno: D OR NOT S.
 * - S: S.
 * - SDno: S OR NOT D.
 * - DSo: D OR S.
 * - 1: Always returns 1 (0xffU).
 *
 * Each operation is implemented as a static function returning the result of the
 * specified bitwise expression.
 * \param revPolish The reverse polish notation name of the raster operation.
 * \param x The expression defining the raster operation using D and S.
 */
#define ROP_REV_POLISH(revPolish, x)                                                                                                                           \
  static blit_scanline_t rop##revPolish(blit_scanline_t fetch, blit_scanline_t store) { return x; }

/*!
 * \brief Raster operation: 0.
 */
ROP_REV_POLISH(0, 0x00U);
/*
 * Note that the macro ends with a semicolon. This is intentional to allow for
 * better formatting, even though semantically it is not required. Without the
 * trailing semicolon, the code would compile correctly, but automated
 * formatting tools will indent the subsequent code incorrectly.
 */

/*!
 * \brief Raster operation: NOT (D OR S).
 */
ROP_REV_POLISH(DSon, ~(D | S));

/*!
 * \brief Raster operation: D AND NOT S.
 */
ROP_REV_POLISH(DSna, D & ~S);

/*!
 * \brief Raster operation: NOT S.
 */
ROP_REV_POLISH(Sn, ~S);

/*!
 * \brief Raster operation: S AND NOT D.
 */
ROP_REV_POLISH(SDna, S & ~D);

/*!
 * \brief Raster operation: NOT D.
 */
ROP_REV_POLISH(Dn, ~D);

/*!
 * \brief Raster operation: D XOR S.
 */
ROP_REV_POLISH(DSx, D ^ S);

/*!
 * \brief Raster operation: NOT (D AND S).
 */
ROP_REV_POLISH(DSan, ~(D & S));

/*!
 * \brief Raster operation: D AND S.
 */
ROP_REV_POLISH(DSa, D & S);

/*!
 * \brief Raster operation: NOT (D XOR S).
 */
ROP_REV_POLISH(DSxn, ~(D ^ S));

/*!
 * \brief Raster operation: D.
 */
ROP_REV_POLISH(D, D);

/*!
 * \brief Raster operation: D OR NOT S.
 */
ROP_REV_POLISH(DSno, D | ~S);

/*!
 * \brief Raster operation: S.
 */
ROP_REV_POLISH(S, S);

/*!
 * \brief Raster operation: S OR NOT D.
 */
ROP_REV_POLISH(SDno, S | ~D);

/*!
 * \brief Raster operation: D OR S.
 */
ROP_REV_POLISH(DSo, D | S);

/*!
 * \brief Raster operation: 1.
 */
ROP_REV_POLISH(1, 0xffU);

/*!
 * \brief Array of raster operation functions.
 * \details This array maps raster operation codes to their corresponding
 * functions. Each function implements a specific raster operation defined
 * using bitwise operations on the source (S) and destination (D) operands.
 */
static blit_rop2_func_t rop2_func[] = {&rop0,   &ropDSon, &ropDSna, &ropSn,   &ropSDna, &ropDn,   &ropDSx, &ropDSan,
                                       &ropDSa, &ropDSxn, &ropD,    &ropDSno, &ropS,    &ropSDno, &ropDSo, &rop1};

static void fetch_logic_mask_store(struct blit_phase_align *align, enum blit_rop2 rop2, blit_scanline_t mask, blit_scanline_t *store) {
  *store = (*store & ~mask) | (mask & rop2_func[rop2](blit_phase_align_fetch(align), *store));
}

static void fetch_logic_store(struct blit_phase_align *align, enum blit_rop2 rop2, blit_scanline_t *store) {
  *store = rop2_func[rop2](blit_phase_align_fetch(align), *store);
}

bool blit_rop2(struct blit_scan *result, struct blit_rgn1 *x, struct blit_rgn1 *y, const struct blit_scan *source, enum blit_rop2 rop2) {
  blit_rgn1_norm(x);
  if (!blit_rgn1_slip(x) || !blit_rgn1_clip(x, result->width - x->origin) || !blit_rgn1_clip(x, source->width - x->origin_source))
    return false;
  blit_rgn1_norm(y);
  if (!blit_rgn1_slip(y) || !blit_rgn1_clip(y, result->height - y->origin) || !blit_rgn1_clip(y, source->height - y->origin_source))
    return false;

  /*
   * Compute some important values up front to avoid doing it inside the bit
   * block transfer loops. The x_max constant represents the maximum x
   * coordinate of the region to be processed. The extra_scan_count constant
   * calculates how many additional bytes (beyond the first byte) are needed to
   * cover the width of the region in bytes. The scan_origin_mask and
   * scan_extent_mask constants are used to mask the bits at the start and end
   * of each scanline, ensuring that only the relevant bits are processed. The
   * offset and offset_source constants are used to calculate the stride offsets
   * for the destination and source scanlines, respectively, allowing for
   * efficient traversal of the scanline buffers.
   */
  const int x_max = x->origin + x->extent - 1;
  const int extra_scan_count = (x_max >> 3) - (x->origin >> 3);
  const blit_scanline_t scan_origin_mask = 0xffU >> (x->origin & 7);
  const blit_scanline_t scan_extent_mask = 0xffU << (7 - (x_max & 7));
  const int offset = result->stride - 1 - extra_scan_count;
  const int offset_source = source->stride - 1 - extra_scan_count;
  blit_scanline_t *store = blit_scan_find(result, x->origin, y->origin);

  /*
   * Set up phase alignment for source fetches. The source x position is given
   * by x->origin_source. The destination x position is given by x->origin. The
   * shift is the difference between these two positions modulo 8. The phase
   * alignment structure handles the bit shifts required to align the source
   * data with the destination data based on the bit positions. Each fetch from
   * the phase alignment structure returns a byte (8 bits) aligned according to
   * the specified bit positions.
   *
   * The & 7 operation here ensures that we are working within the bounds of a
   * byte (0-7 bits). This is important because the phase alignment also
   * corrects for source byte offsets, ensuring that the fetched data is
   * correctly aligned with respect to the destination, regardless of the
   * starting bit positions. Do not let the phase alignment and the scan find
   * get out of sync! Keep them in step!
   */
  struct blit_phase_align align;
  blit_phase_align_start(&align, x->origin, x->origin_source & 7, blit_scan_find(source, x->origin_source, y->origin_source));

  if (extra_scan_count == 0) {
    const blit_scanline_t scan_mask = scan_origin_mask & scan_extent_mask;
    int extent = y->extent;
    while (extent--) {
      blit_phase_align_prefetch(&align);
      fetch_logic_mask_store(&align, rop2, scan_mask, store++);
      store += offset;
      align.store += offset_source;
    }
  } else {
    int extent = y->extent;
    while (extent--) {
      blit_phase_align_prefetch(&align);
      fetch_logic_mask_store(&align, rop2, scan_origin_mask, store++);
      int count = extra_scan_count;
      while (--count) {
        fetch_logic_store(&align, rop2, store++);
      }
      fetch_logic_mask_store(&align, rop2, scan_extent_mask, store++);
      store += offset;
      align.store += offset_source;
    }
  }
  return true;
}

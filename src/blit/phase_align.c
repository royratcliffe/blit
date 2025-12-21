/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

/*!
 * \file phase_align.c
 * \brief Phase alignment for 8-bit bytes.
 * This source file implements the functions for managing phase alignment of
 * 8-bit bytes, as defined in the `phase_align.h` header file.
 */

#include <blit/phase_align.h>

static void prefetch(struct blit_phase_align *align);
static void prefetch_left_shift(struct blit_phase_align *align);

/*
 * MISRA-C compliance for rule 8.4: static functions should be declared before
 * use. The functions `fetch_left_shift`, `fetch`, and `fetch_right_shift` are
 * declared before they are defined to ensure that they can be used within the
 * `phase_align_start` and `phase_align_fetch` functions without any issues.
 * This is necessary to avoid implicit function declarations, which can lead to
 * undefined behavior and is against the C11 standard.
 */
static blit_scanline_t fetch_left_shift(struct blit_phase_align *align);
static blit_scanline_t fetch(struct blit_phase_align *align);
static blit_scanline_t fetch_right_shift(struct blit_phase_align *align);

/*
 * Decide how to fetch the source bits. There are three phase-align functions to
 * choose from, based on how the bits are out of phase. The destination
 * alignment is x & 7, i.e. how many bits from the left side of the byte.
 * Expression x_store & 7 gives the source alignment. The sign and magnitude of
 * the difference between the alignments determines the direction and amount of
 * shift.
 *
 * Note that only the least significant three bits of x are used since the
 * output is always a byte (8 bits). The shift value is calculated based on the
 * difference between the destination x and the source x_store. The output
 * stream appears as a sequence of bytes returned by the phase_align_fetch
 * function, where each byte is shifted according to the specified shift value.
 */
void blit_phase_align_start(struct blit_phase_align *align, int x, int x_store, const blit_scanline_t *store) {
  align->store = store + (x_store >> 3);
  int shift = (x & 7) - (x_store & 7);
  if (shift < 0) {
    align->prefetch = &prefetch_left_shift;
    align->fetch = &fetch_left_shift;
    align->shift = -shift;
    align->carry = *align->store;
  } else if (shift == 0) {
    align->prefetch = &prefetch;
    align->fetch = &fetch;
    /*
     * Shift and carry are not used when there is no shift.
     * Be tidy and set them to zero.
     */
    align->shift = 0;
    align->carry = 0x00U;
  } else {
    /*
     * Carry starts off as undefined for right shifts. Make it zero so that
     * the first byte fetched does not have any undefined overflow from the
     * previous non-byte.
     */
    align->prefetch = &prefetch;
    align->fetch = &fetch_right_shift;
    align->shift = shift;
    align->carry = 0x00U;
  }
}

void blit_phase_align_prefetch(struct blit_phase_align *align) { (*align->prefetch)(align); }

blit_scanline_t blit_phase_align_fetch(struct blit_phase_align *align) { return (*align->fetch)(align); }

static void prefetch(struct blit_phase_align *align) { (void)align; }

static void prefetch_left_shift(struct blit_phase_align *align) { align->carry = *align->store; }

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with a left shift.
 * \param align Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
static blit_scanline_t fetch_left_shift(struct blit_phase_align *align) {
  const blit_scanline_t lo = *++align->store; /* pre-increment */
  const blit_scanline_t hi = align->carry;    /* carry is the previous value */
  align->carry = lo;                          /* store the current value as carry for the next call */
  return (hi << align->shift) | (lo >> (8 - align->shift));
}

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with no shift.
 * \param align Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
static blit_scanline_t fetch(struct blit_phase_align *align) { return *align->store++; }

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with a right shift.
 * \param align Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
static blit_scanline_t fetch_right_shift(struct blit_phase_align *align) {
  const uint8_t lo = *align->store++; /* post-increment */
  const uint8_t hi = align->carry;    /* carry is the previous value */
  align->carry = lo;                  /* store the current value as carry for the next call */
  return (hi << (8 - align->shift)) | (lo >> align->shift);
}

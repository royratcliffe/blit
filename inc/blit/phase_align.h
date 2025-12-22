/*
 * SPDX-FileCopyrightText: 2025, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

#ifndef __BLIT_PHASE_ALIGN_H__
#define __BLIT_PHASE_ALIGN_H__

/*!
 * \file blit/phase_align.h
 * \brief Phase alignment for 8-bit bytes.
 * \details This header file defines the `phase_align` structure and functions
 * for managing phase alignment of 8-bit bytes. It allows for fetching bytes
 * with a specified shift, which is useful in scenarios where data needs to be
 * aligned or shifted for processing, such as in graphics or display memory
 * management.
 */

#include <blit/scan.h>

/*!
 * \brief Phase alignment structure.
 * \details This structure is used to manage phase alignment for 8-bit bytes. It
 * contains a function pointer for fetching bytes, a pointer to the store, a
 * carry value, and a shift count.
 *
 * The `fetch` function pointer is used to retrieve the next byte in the
 * phase-aligned stream, either by left shifting, right shifting, or no
 * shifting. The `store` pointer points to the data being processed. The `shift`
 * value indicates how many bits to shift the data. The `carry`
 * value is used to hold any overflow from the previous byte fetch.
 */
struct blit_phase_align {
  /*!
   * \brief Prefetch the next byte into the alignment structure.
   */
  void (*prefetch)(struct blit_phase_align *align);
  /*!
   * \brief Fetch the next byte with appropriate shifting.
   */
  blit_scanline_t (*fetch)(struct blit_phase_align *align);
  /*!
   * \brief Pointer to the data being processed.
   */
  const blit_scanline_t *store;
  /*!
   * \brief Number of bits to shift the data.
   */
  int shift;
  /*!
   * \brief Overflow from the previous byte fetch.
   */
  blit_scanline_t carry;
};

/*!
 * \brief Initialises the phase alignment structure.
 * \details This function sets up the phase alignment structure with the given
 * parameters.
 * \param align Pointer to the phase alignment structure.
 * \param x The destination bit position.
 * \param x_store The source bit position.
 * \param store Pointer to the data buffer.
 */
void blit_phase_align_start(struct blit_phase_align *align, int x, int x_store,
                            const blit_scanline_t *store);

/*!
 * \brief Prefetches the next byte into the alignment structure.
 * \details This function prepares the phase alignment structure for the next
 * byte fetch.
 * \param align Pointer to the phase alignment structure.
 */
void blit_phase_align_prefetch(struct blit_phase_align *align);

/*!
 * \brief Fetches the next byte from the phase alignment structure.
 * \param align Pointer to the phase alignment structure.
 * \return The fetched byte.
 */
blit_scanline_t blit_phase_align_fetch(struct blit_phase_align *align);

/*!
 * \brief Fetches a byte from a stored buffer.
 * \param x_store The source bit position relative to the given start of the
 * buffer, where 0 is the most-significant bit of the first byte.
 * \param store Pointer to the data buffer.
 * \return The fetched byte.
 */
static inline uint8_t blit_phase_align_byte(int x_store,
                                            const blit_scanline_t *store) {
  struct blit_phase_align align;
  blit_phase_align_start(&align, 0, x_store, store);
  return blit_phase_align_fetch(&align);
}

/*!
 * \brief Fetches a 16-bit big-endian value from a stored buffer.
 * \param x_store Bit position in the source buffer (0 is MSB of first byte).
 * \param store Pointer to the data buffer.
 * \return The fetched 16-bit value.
 */
static inline uint16_t blit_phase_align_be16(int x_store,
                                             const blit_scanline_t *store) {
  struct blit_phase_align align;
  blit_phase_align_start(&align, 0, x_store, store);
  return (blit_phase_align_fetch(&align) << 8) | blit_phase_align_fetch(&align);
}

/*!
 * \brief Fetches a 16-bit little-endian value from a stored buffer.
 * \param x_store Bit position in the source buffer (0 is MSB of first byte).
 * \param store Pointer to the data buffer.
 * \return The fetched 16-bit value.
 */
static inline uint16_t blit_phase_align_le16(int x_store,
                                             const blit_scanline_t *store) {
  struct blit_phase_align align;
  blit_phase_align_start(&align, 0, x_store, store);
  return blit_phase_align_fetch(&align) | (blit_phase_align_fetch(&align) << 8);
}

/*!
 * \brief Fetches a 32-bit big-endian value from a stored buffer.
 * \param x_store Bit position in the source buffer (0 is MSB of first byte).
 * \param store Pointer to the data buffer.
 * \return The fetched 32-bit value.
 */
static inline uint32_t blit_phase_align_be32(int x_store,
                                             const blit_scanline_t *store) {
  struct blit_phase_align align;
  blit_phase_align_start(&align, 0, x_store, store);
  return (blit_phase_align_fetch(&align) << 24) |
         (blit_phase_align_fetch(&align) << 16) |
         (blit_phase_align_fetch(&align) << 8) | blit_phase_align_fetch(&align);
}

/*!
 * \brief Fetches a 32-bit little-endian value from a stored buffer.
 * \param x_store Bit position in the source buffer (0 is MSB of first byte).
 * \param store Pointer to the data buffer.
 * \return The fetched 32-bit value.
 */
static inline uint32_t blit_phase_align_le32(int x_store,
                                             const blit_scanline_t *store) {
  struct blit_phase_align align;
  blit_phase_align_start(&align, 0, x_store, store);
  return blit_phase_align_fetch(&align) |
         (blit_phase_align_fetch(&align) << 8) |
         (blit_phase_align_fetch(&align) << 16) |
         (blit_phase_align_fetch(&align) << 24);
}

#endif /* __BLIT_PHASE_ALIGN_H__ */

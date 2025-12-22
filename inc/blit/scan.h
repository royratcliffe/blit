/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */
/*!
 * \file blit/scan.h
 * \brief Scanline structure definition.
 * \details This header file defines the `blit_scan` structure, which represents
 * a scanline buffer used in graphics operations. The structure includes
 * information about the buffer's storage, dimensions, and stride.
 */

#ifndef __BLIT_SCAN_H__
#define __BLIT_SCAN_H__

#include <stdint.h>

/*!
 * \brief Type definition for a scanline element.
 * \details The `blit_scanline_t` type represents a single element in a scanline
 * buffer. It is defined as an 8-bit unsigned integer, which allows for
 * efficient storage and manipulation of pixel data.
 */
typedef uint8_t blit_scanline_t;

/*!
 * \brief Scanline structure.
 * \details The `blit_scan` structure represents a scanline buffer used in
 * graphics operations. It contains information about the buffer's storage,
 * dimensions, and stride.
 */
struct blit_scan {
  /*!
   * \brief Pointer to the scanline data buffer.
   * \details The `store` pointer points to the beginning of the scanline data
   * buffer. The buffer is organised as a contiguous array of `blit_scanline_t`
   * elements.
   */
  blit_scanline_t *store;
  /*!
   * \brief Width of the scanline buffer in pixels.
   * \details The width represents the number of bit elements in each row of the
   * scanline buffer.
   */
  int width;
  /*!
   * \brief Height of the scanline buffer in pixels.
   * \details The height represents the number of rows in the scanline buffer.
   */
  int height;
  /*!
   * \brief Stride of the scanline buffer.
   * \details The number of bytes between the start of each row in the scanline
   * buffer. The number must be positive (hence the unsigned size type) and at
   * least \c width bytes.
   */
  int stride;
};

/*!
 * \brief Macro to define a scanline structure with storage.
 * \details This macro defines a scanline structure along with its associated
 * storage buffer. The macro takes the name of the scanline structure, its
 * width, and its height as parameters. It creates a storage array of
 * `blit_scanline_t` elements and initialises a `blit_scan` structure with the
 * appropriate values.
 * \param name The name of the scanline structure.
 * \param width The width of the scanline buffer in pixels.
 * \param height The height of the scanline buffer in pixels.
 * \note The stride is automatically calculated based on the width, ensuring
 * that it is sufficient to hold the specified number of pixels.
 */
#define BLIT_SCAN_DEFINE(name, width, height)                                                                                                                  \
  blit_scanline_t name##_store[(((width) + 7) >> 3) * (height)];                                                                                               \
  struct blit_scan name = {name##_store, (width), (height), ((width) + 7) >> 3}

/*!
 * \brief Macro to define a static scanline structure with storage.
 * \details This macro defines a static scanline structure along with its
 * associated storage buffer. The macro takes the name of the scanline structure,
 * its width, and its height as parameters. It creates a static storage array of
 * `blit_scanline_t` elements and initialises a static `blit_scan` structure with the
 * appropriate values.
 * \param name The name of the static scanline structure.
 * \param width The width of the scanline buffer in pixels.
 * \param height The height of the scanline buffer in pixels.
 * \note The stride is automatically calculated based on the width, ensuring
 * that it is sufficient to hold the specified number of pixels.
 */
#define BLIT_SCAN_DEFINE_STATIC(name, width, height)                                                                                                           \
  static blit_scanline_t name##_store[(((width) + 7) >> 3) * (height)];                                                                                        \
  static struct blit_scan name = {name##_store, (width), (height), ((width) + 7) >> 3}

/*!
 * \brief Find the pointer to a specific bit in the scanline buffer.
 * \details This inline function calculates the pointer to a specific bit in the
 * scanline buffer based on the given x and y coordinates. The x coordinate
 * specifies the bit position within the row, while the y coordinate specifies
 * the row number. The function returns a pointer to the corresponding
 * `blit_scanline_t` element in the buffer.
 * \param scan Pointer to the scanline structure.
 * \param x The x coordinate (bit position) within the row.
 * \param y The y coordinate (row number).
 * \return Pointer to the corresponding `blit_scanline_t` element.
 */
static inline blit_scanline_t *blit_scan_find(const struct blit_scan *scan, int x, int y) { return scan->store + scan->stride * y + (x >> 3); }

#endif /* __BLIT_SCAN_H__ */

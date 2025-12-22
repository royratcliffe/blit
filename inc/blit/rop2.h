/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

 /*!
  * \file blit/rop2.h
  * \brief Binary raster operations.
  * \details This header file declares the enumeration of binary raster operation
  * codes used in graphics operations, as well as the function prototype for
  * performing raster operations on scan structures. These operations combine
  * source and destination pixel values using bitwise operations.
  */

#ifndef __BLIT_ROP2_H__
#define __BLIT_ROP2_H__

#include <blit/rgn1.h>
#include <blit/scan.h>

/*!
 * \brief Enumeration of binary raster operation codes.
 * \details This enumeration defines various binary raster operation codes
 * used in graphics operations. Each code corresponds to a specific raster
 * operation that combines source and destination pixel values using
 * bitwise operations.
 */
enum blit_rop2 {
  blit_rop2_0,
  blit_rop2_DSon,
  blit_rop2_DSna,
  blit_rop2_Sn,
  blit_rop2_SDna,
  blit_rop2_Dn,
  blit_rop2_DSx,
  blit_rop2_DSan,
  blit_rop2_DSa,
  blit_rop2_DSxn,
  blit_rop2_D,
  blit_rop2_DSno,
  blit_rop2_S,
  blit_rop2_SDno,
  blit_rop2_DSo,
  blit_rop2_1,
  blit_rop2_not_erase = blit_rop2_DSon,
  blit_rop2_not_copy = blit_rop2_Sn,
  blit_rop2_erase = blit_rop2_SDna,
  blit_rop2_xor = blit_rop2_DSx,
  blit_rop2_and = blit_rop2_DSa,
  blit_rop2_merge_paint = blit_rop2_DSno,
  blit_rop2_copy = blit_rop2_S,
  blit_rop2_paint = blit_rop2_DSo,
  /*
   * Unary raster operations defined in terms of binary operations.
   */
  blit_rop2_blackness = blit_rop2_0,
  blit_rop2_invert = blit_rop2_Sn,
  blit_rop2_whiteness = blit_rop2_1,
};

/*!
 * \brief Perform raster operation with masking and store the result.
 * \details This function performs a raster operation defined by the \c rop2
 * parameter on a specified region of the source and destination scan
 * structures. It uses one-dimensional region structures for both the x and y
 * axes to define the area of operation. The function handles phase alignment of
 * the source data to ensure correct bit alignment during the operation.
 * \param result Pointer to the destination scan structure.
 * \param x Pointer to the one-dimensional region structure for the x-axis.
 * \param y Pointer to the one-dimensional region structure for the y-axis.
 * \param source Pointer to the source scan structure.
 * \param rop2 The raster operation code.
 * \return true if the operation was successful, false otherwise.
 */
bool blit_rop2(struct blit_scan *result, struct blit_rgn1 *x, struct blit_rgn1 *y, const struct blit_scan *source, enum blit_rop2 rop2);

#endif /* __BLIT_ROP2_H__ */

/*
 * SPDX-FileCopyrightText: 2025, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */
/*!
 * \file blit/rgn1.h
 * \brief One-dimensional region structure.
 * \details This header file defines the \c blit_rgn1 structure, which
 * represents a one-dimensional region with an origin, extent, and source
 * origin. It also provides inline functions for normalising, slipping, and
 * clipping the region.
 */

#ifndef __BLIT_RGN1_H__
#define __BLIT_RGN1_H__

#include <assert.h>
#include <stdbool.h>

/*!
 * \brief One-dimensional region structure.
 * \details The \c blit_rgn1 structure represents a one-dimensional region with
 * an origin, extent, and source origin. It is used to define regions in
 * one-dimensional space for various operations.
 */
struct blit_rgn1 {
  /*!
   * \brief Origin of the region.
   */
  int origin;
  /*!
   * \brief Extent of the region.
   */
  int extent;
  /*!
   * \brief Source origin of the region.
   */
  int origin_source;
};

/*!
 * \brief Normalise a one-dimensional region.
 * \details This function normalizes a one-dimensional region represented by the
 * \c blit_rgn1 structure. If the extent of the region is negative, it
 * adjusts the origin and origin_source accordingly to ensure that the
 * extent is non-negative.
 * \param rgn1 Pointer to the \c blit_rgn1 structure to normalise.
 */
static inline void blit_rgn1_norm(struct blit_rgn1 *rgn1) {
  /*
   * Normalise the extents. Extents are normally positive. A negative extent
   * means the destination and source origins specify the far edge of the
   * rectangle. Two's-complement negative extents and put the origins at the
   * rectangle's origins.
   */
  if (rgn1->extent < 0) {
    rgn1->extent = -rgn1->extent;
    rgn1->origin -= rgn1->extent;
    rgn1->origin_source -= rgn1->extent;
  }
  assert(rgn1->extent >= 0);
}

/*!
 * \brief Slip a one-dimensional region into positive space.
 * \details This function adjusts a one-dimensional region represented by the
 * \c blit_rgn1 structure to ensure that both the origin and source origin are
 * non-negative. If either the origin or source origin is negative, the region
 * is "slipped" into positive space by adjusting the origins and reducing the
 * extent accordingly. If the entire region is outside positive space, the
 * function returns false.
 * \param rgn1 Pointer to the \c blit_rgn1 structure to slip.
 * \retval true if the region was successfully slipped into positive space.
 * \retval false if the entire region is outside positive space.
 */
static inline bool blit_rgn1_slip(struct blit_rgn1 *rgn1) {
  int offset = rgn1->origin < 0
                   ? (rgn1->origin < rgn1->origin_source ? -rgn1->origin
                                                         : -rgn1->origin_source)
                   : (rgn1->origin_source < 0 ? -rgn1->origin_source : 0);
  assert(offset >= 0);
  if (offset >= rgn1->extent)
    return false;
  rgn1->origin += offset;
  rgn1->origin_source += offset;
  rgn1->extent -= offset;
  assert(rgn1->origin >= 0 && rgn1->origin_source >= 0 && 0 < rgn1->extent);
  return true;
}

/*!
 * \brief Clip a one-dimensional region to a given extent.
 * \details This function clips a one-dimensional region represented by the
 * \c blit_rgn1 structure to a specified extent. If the current extent of the
 * region exceeds the given extent, it is reduced accordingly. If the given
 * extent is non-positive, the function returns false.
 * \param rgn1 Pointer to the \c blit_rgn1 structure to clip.
 * \param extent The extent to which the region should be clipped.
 * \return true if the region was successfully clipped; false if the given
 * extent is non-positive.
 */
static inline bool blit_rgn1_clip(struct blit_rgn1 *rgn1, int extent) {
  if (0 >= extent)
    return false;
  if (extent < rgn1->extent)
    rgn1->extent = extent;
  return true;
}

#endif /* __BLIT_RGN1_H__ */

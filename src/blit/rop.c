#include <blit/scan.h>

/*!
 * \brief 8-bit destination operand.
 */
#define D (*store)

/*!
 * \brief 8-bit source operand.
 */
#define S fetch()

#define ROP_REV_POLISH(revPolish, x)                                                                                   \
  scan_byte rop##revPolish(const scan_byte *store, scan_byte (*fetch)(void)) { return x; }

/*!
 * \brief Raster operation: 0.
 */
ROP_REV_POLISH(0, 0x00U);

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
ROP_REV_POLISH(DSan, ~(D &S));

/*!
 * \brief Raster operation: D AND S.
 */
ROP_REV_POLISH(DSa, D &S);

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

scan_byte (*rop[])(const scan_byte *store, scan_byte (*fetch)(void)) = {
    &rop0,   &ropDSon, &ropDSna, &ropSn,   &ropSDna, &ropDn,   &ropDSx, &ropDSan,
    &ropDSa, &ropDSxn, &ropD,    &ropDSno, &ropS,    &ropSDno, &ropDSo, &rop1,
};

#include <blit/scan.h>

/*!
 * \brief 8-bit source operand.
 */
#define S fetch()

/*!
 * \brief 8-bit destination operand.
 */
#define D (*store)

typedef blit_scan_t (*blit_rop_func_t)(blit_scan_t (*fetch)(void), const blit_scan_t *store);

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
#define ROP_REV_POLISH(revPolish, x)                                                                                   \
  static blit_scan_t rop##revPolish(blit_scan_t (*fetch)(void), const blit_scan_t *store) { return x; }

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

blit_rop_func_t rop[] = {
    &rop0,   &ropDSon, &ropDSna, &ropSn,   &ropSDna, &ropDn,   &ropDSx, &ropDSan,
    &ropDSa, &ropDSxn, &ropD,    &ropDSno, &ropS,    &ropSDno, &ropDSo, &rop1,
};

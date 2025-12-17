#ifndef _MY_MATH_H_
#define _MY_MATH_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <at32f403a_407.h>

    // typedef struct int48_e
    // {
    //     bool sign : 1;
    //     uint64_t value : 47;
    // } int48_t;

    // #ifndef abs
    // #define abs( x )    ( ( x ) > 0 ? ( x ) : -( x ) )
    // #endif

    // long map(long x, long in_min, long in_max, long out_min, long out_max) {
    //   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    // }

    /**
     * @brief Extract bits from a value by shifting right and applying mask
     * @param value The input value
     * @param shift Number of bits to shift right
     * @param mask Bit mask to apply after shifting
     * @return Extracted bits
     * 
     * @example EXTRACT_BITS(0x1ABC, 8, 0x0F) returns 0x0A (bits 8-11)
     * @example EXTRACT_BITS(buffer->ident, 11, 0x0F) extracts DLC from CANopen ident
     * 
     * @note Type-safe and generates efficient code
     * @note Works with any integer type (uint8_t, uint16_t, uint32_t, etc.)
     */
    #define EXTRACT_BITS(value, shift, mask) \
        (((__typeof__(value))((value) >> (shift))) & (mask))

    float myAbs(float d);

#ifdef __cplusplus
}
#endif

#endif //_MY_MATH_H_
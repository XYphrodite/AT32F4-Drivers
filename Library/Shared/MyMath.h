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

    float myAbs(float d);

#ifdef __cplusplus
}
#endif

#endif //_MY_MATH_H_
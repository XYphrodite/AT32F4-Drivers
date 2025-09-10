#ifndef _CORE_H_
#define _CORE_H_

#include "cmsis_gcc.h"

#define interrupts()      __set_PRIMASK(0)
#define noInterrupts()    __set_PRIMASK(1)

#endif //_CORE_H_
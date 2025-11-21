#ifndef _SIMPLE_WD_H_
#define _SIMPLE_WD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "at32f403a_407.h"


    void simple_wdt_init(void);
    void simple_wdt_reload(void);

#ifdef __cplusplus
}
#endif

#endif

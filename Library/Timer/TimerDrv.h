#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "at32f403a_407_tmr.h"

/* Define timer base init structure if not provided by vendor headers */
#ifndef __TMR_BASE_INIT_TYPE_DEFINED
#define __TMR_BASE_INIT_TYPE_DEFINED
    typedef struct
    {
        uint32_t period;
        uint32_t div;
        uint32_t count_mode;
        uint32_t clock_division;
        uint32_t repetition_counter;
    } tmr_base_init_type;
#endif

/* Initialize timer using tmr_base_init_type */
void tmr_apply_base_init(tmr_type* tmr_x, const tmr_base_init_type* init);

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_DRV_H_ */



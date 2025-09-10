#ifndef _MODEM_TMR_H_
#define _MODEM_TMR_H_

#include "at32f403a_407.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    void init_modem_tmr(void);
    void wk_tmr2_init(void);
    void start_modem_tmr(uint32_t ms, void (*callback)(void* context), void* context);
    void stop_modem_tmr(void);
    void TMR2_GLOBAL_IRQHandler(void);
    void tmr_irq_enable(void);
    void tmr_irq_disable(void);
    void tmr_check_irq(void);

#ifdef __cplusplus
}
#endif


#endif
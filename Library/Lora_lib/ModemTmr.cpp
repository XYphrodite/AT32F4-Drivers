#include "ModemTmr.h"

uint32_t modem_timer_delay_ms;
void (*modem_timer_callback)(void* context);
void* modem_context;
bool tmrInterruptEn = false;

void init_modem_tmr(void) {
    modem_timer_delay_ms = -1;
    wk_tmr2_init();
}

/**
  * @brief  init tmr2 function.
  * @param  none
  * @retval none
  */
void wk_tmr2_init(void) {
    /* enable tmr2 periph clock */
    crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
    /* configure counter settings */
    tmr_base_init(TMR2, 3999, 59); //240MHz / ( ( 3999 + 1 ) * ( 59 + 1 ) ) = 1000Hz = 1ms
    tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR2, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(TMR2, FALSE);

    /* configure primary mode settings */
    tmr_sub_sync_mode_set(TMR2, FALSE);
    tmr_primary_mode_select(TMR2, TMR_PRIMARY_SEL_RESET);

    nvic_irq_enable(TMR2_GLOBAL_IRQn, 4, 0);
    tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
    tmr_irq_enable();
}

void start_modem_tmr(uint32_t ms, void (*callback)(void* context), void* context) {
    modem_timer_delay_ms = ms;
    modem_timer_callback = callback;
    modem_context = context;
    tmr_counter_enable(TMR2, TRUE);
}

void stop_modem_tmr(void) {
    tmr_counter_enable(TMR2, FALSE);
    modem_timer_delay_ms = -1;
}

void TMR2_GLOBAL_IRQHandler(void) {
    if (tmr_flag_get(TMR2, TMR_OVF_FLAG) != RESET) {
        tmr_flag_clear(TMR2, TMR_OVF_FLAG);
        if (modem_timer_delay_ms != 0)
            modem_timer_delay_ms--;
        else {
            tmr_check_irq();
        }
    }
}

void tmr_irq_enable(void) {
    tmrInterruptEn = true;
}

void tmr_irq_disable(void) {
    tmrInterruptEn = false;
}

void tmr_check_irq(void) {
    if (tmrInterruptEn && modem_timer_delay_ms == 0) {
        if (modem_timer_callback != NULL) {
            modem_timer_callback(modem_context);
        }
        stop_modem_tmr();
    }
}
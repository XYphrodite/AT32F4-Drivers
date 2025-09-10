#include "UpTime.h"

uint32_t upTime_s = 0;
uint32_t upTime_ms = 0;
// uint32_t upTime_100us = 0;

void init_UpTime_Tmr(void)
{
    /* enable tmr1 periph clock */
    crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

    /* configure counter settings */
    // tmr_base_init(TMR1, 3999, 59); // 240MHz / ( ( 3999 + 1 ) * ( 59 + 1 ) ) = 1000Hz = 1ms
    tmr_base_init(TMR1, 499, 23); // 12MHz / ( ( 499 + 1 ) * ( 23 + 1 ) ) = 1000Hz = 1ms
    tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
    tmr_repetition_counter_set(TMR1, 0);
    tmr_period_buffer_enable(TMR1, FALSE);

    /* configure primary mode settings */
    tmr_sub_sync_mode_set(TMR1, FALSE);
    tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_RESET);

    tmr_counter_enable(TMR1, TRUE);

    nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 4, 0);
    tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
}

void TMR1_OVF_TMR10_IRQHandler(void)
{
    if (tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
    {
        tmr_flag_clear(TMR1, TMR_OVF_FLAG);
        upTime_ms++; // overflow
        if (upTime_ms % 1000 == 0)
        {
            upTime_s++;
        }
        // upTime_100us++;
        // if (upTime_100us % 10 == 0) {
        //     upTime_ms++;
        // }
        // if (upTime_100us % 10000 == 0) {
        //     upTime_s++;
        // }
    }
}

uint32_t GetUpTimeMs(void) { return upTime_ms; }
uint32_t GetUpTimeS(void) { return upTime_s; }
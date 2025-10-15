#include "TimerDrv.h"
#include "at32f403a_407_clock.h"
#include <stddef.h>

/* External reference to system core clock */
// extern uint32_t system_core_clock;

void drv_timer_init(const timer_init_type* init)
{
    if (init == NULL || init->timer == NULL)
    {
        return;
    }

    /* Extract parameters */
    tmr_type* timer = init->timer;
    uint32_t period_us = init->period_us;
    
    /* Enable timer peripheral clock */
    if (init->timer_clk != 0)
    {
        crm_periph_clock_enable(init->timer_clk, TRUE);
    }

    /* Calculate timer configuration for desired period in microseconds
     * Timer clock = system_core_clock / (prescaler + 1)
     * For microsecond resolution, we want 1MHz timer clock
     * Prescaler = (system_core_clock / 1000000) - 1
     * Period = period_us - 1 (for timer counting from 0)
     */
    uint32_t prescaler = (system_core_clock / 1000000U) - 1U;
    uint32_t period = period_us - 1U;

    /* Configure timer base */
    tmr_base_init(timer, period, prescaler);
    tmr_cnt_dir_set(timer, init->count_mode);
    tmr_clock_source_div_set(timer, init->clock_div);
    
    /* Configure repetition counter for advanced timers */
    if (init->repetition_counter > 0)
    {
        tmr_repetition_counter_set(timer, init->repetition_counter);
    }

    /* Reset counter to 0 */
    tmr_counter_value_set(timer, 0);

    /* Configure NVIC for timer interrupt */
    if (init->timer_irq != (IRQn_Type)0)
    {
        nvic_irq_enable(init->timer_irq, init->irq_priority, init->irq_subpriority);
    }

    /* Timer is initialized but not started - application must call drv_timer_enable() */
}

void drv_timer_enable(tmr_type* timer, confirm_state enable)
{
    if (timer == NULL)
    {
        return;
    }
    
    tmr_counter_enable(timer, enable);
}

void drv_timer_interrupt_enable(tmr_type* timer, timer_interrupt_type int_type, confirm_state enable)
{
    if (timer == NULL)
    {
        return;
    }

    /* Map custom interrupt type to HAL flags */
    if (int_type & TIMER_INT_OVERFLOW)
    {
        tmr_interrupt_enable(timer, TMR_OVF_INT, enable);
    }
    if (int_type & TIMER_INT_CHANNEL1)
    {
        tmr_interrupt_enable(timer, TMR_C1_INT, enable);
    }
    if (int_type & TIMER_INT_CHANNEL2)
    {
        tmr_interrupt_enable(timer, TMR_C2_INT, enable);
    }
    if (int_type & TIMER_INT_CHANNEL3)
    {
        tmr_interrupt_enable(timer, TMR_C3_INT, enable);
    }
    if (int_type & TIMER_INT_CHANNEL4)
    {
        tmr_interrupt_enable(timer, TMR_C4_INT, enable);
    }
}

uint32_t drv_timer_get_counter(tmr_type* timer)
{
    if (timer == NULL)
    {
        return 0;
    }
    
    return tmr_counter_value_get(timer);
}

void drv_timer_set_counter(tmr_type* timer, uint32_t value)
{
    if (timer == NULL)
    {
        return;
    }
    
    tmr_counter_value_set(timer, value);
}

void drv_timer_clear_flag(tmr_type* timer, timer_interrupt_type int_type)
{
    if (timer == NULL)
    {
        return;
    }

    /* Map custom interrupt type to HAL flags */
    if (int_type & TIMER_INT_OVERFLOW)
    {
        tmr_flag_clear(timer, TMR_OVF_FLAG);
    }
    if (int_type & TIMER_INT_CHANNEL1)
    {
        tmr_flag_clear(timer, TMR_C1_FLAG);
    }
    if (int_type & TIMER_INT_CHANNEL2)
    {
        tmr_flag_clear(timer, TMR_C2_FLAG);
    }
    if (int_type & TIMER_INT_CHANNEL3)
    {
        tmr_flag_clear(timer, TMR_C3_FLAG);
    }
    if (int_type & TIMER_INT_CHANNEL4)
    {
        tmr_flag_clear(timer, TMR_C4_FLAG);
    }
}

confirm_state drv_timer_get_flag(tmr_type* timer, timer_interrupt_type int_type)
{
    if (timer == NULL)
    {
        return FALSE;
    }

    /* Check for overflow flag (most common case) */
    if (int_type & TIMER_INT_OVERFLOW)
    {
        return (tmr_flag_get(timer, TMR_OVF_FLAG) == SET) ? TRUE : FALSE;
    }
    if (int_type & TIMER_INT_CHANNEL1)
    {
        return (tmr_flag_get(timer, TMR_C1_FLAG) == SET) ? TRUE : FALSE;
    }
    if (int_type & TIMER_INT_CHANNEL2)
    {
        return (tmr_flag_get(timer, TMR_C2_FLAG) == SET) ? TRUE : FALSE;
    }
    if (int_type & TIMER_INT_CHANNEL3)
    {
        return (tmr_flag_get(timer, TMR_C3_FLAG) == SET) ? TRUE : FALSE;
    }
    if (int_type & TIMER_INT_CHANNEL4)
    {
        return (tmr_flag_get(timer, TMR_C4_FLAG) == SET) ? TRUE : FALSE;
    }

    return FALSE;
}

/* Legacy function - kept for backward compatibility */
void tmr_apply_base_init(tmr_type* tmr_x, const tmr_base_init_type* init)
{
    if (tmr_x == NULL || init == NULL)
    {
        return;
    }

    tmr_base_init(tmr_x, init->period, init->div);
    tmr_cnt_dir_set(tmr_x, (tmr_count_mode_type)init->count_mode);
    tmr_clock_source_div_set(tmr_x, (tmr_clock_division_type)init->clock_division);
    tmr_repetition_counter_set(tmr_x, (uint8_t)init->repetition_counter);
}



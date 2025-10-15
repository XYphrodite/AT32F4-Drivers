#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "at32f403a_407_tmr.h"
#include "at32f403a_407_crm.h"

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

/* Timer interrupt type enumeration - BIT FLAGS (can be OR'ed together) */
typedef enum
{
    TIMER_INT_OVERFLOW = (1U << 0),      /*!< Timer overflow interrupt (0x01) */
    TIMER_INT_CHANNEL1 = (1U << 1),      /*!< Timer channel 1 interrupt (0x02) */
    TIMER_INT_CHANNEL2 = (1U << 2),      /*!< Timer channel 2 interrupt (0x04) */
    TIMER_INT_CHANNEL3 = (1U << 3),      /*!< Timer channel 3 interrupt (0x08) */
    TIMER_INT_CHANNEL4 = (1U << 4),      /*!< Timer channel 4 interrupt (0x10) */
    TIMER_INT_ALL      = 0x1FU           /*!< All timer interrupts (0x1F) */
} timer_interrupt_type;

/* Enhanced timer initialization structure */
#ifndef __TIMER_INIT_TYPE_DEFINED
#define __TIMER_INIT_TYPE_DEFINED
typedef struct
{
    tmr_type* timer;                        /*!< Timer peripheral (TMR1, TMR2, etc.) */
    uint32_t period_us;                     /*!< Desired period in microseconds */
    tmr_count_mode_type count_mode;         /*!< Count up/down mode */
    tmr_clock_division_type clock_div;      /*!< Clock division factor */
    uint8_t repetition_counter;             /*!< Repetition counter (for advanced timers) */
    
    /* IRQ configuration */
    IRQn_Type timer_irq;                    /*!< IRQ number for timer */
    uint8_t irq_priority;                   /*!< IRQ priority (0-15) */
    uint8_t irq_subpriority;                /*!< IRQ sub-priority */
    
    /* Clock enable */
    crm_periph_clock_type timer_clk;        /*!< Timer peripheral clock */
} timer_init_type;
#endif

/// @brief Initialize timer with flexible configuration
/// @param init Pointer to timer_init_type configuration structure
void drv_timer_init(const timer_init_type* init);

/// @brief Enable or disable timer counter
/// @param timer Timer peripheral to control
/// @param enable TRUE to enable, FALSE to disable
void drv_timer_enable(tmr_type* timer, confirm_state enable);

/// @brief Enable or disable timer interrupt
/// @param timer Timer peripheral to control
/// @param int_type Interrupt type to enable/disable (bit flags, can be OR'ed: TIMER_INT_OVERFLOW | TIMER_INT_CHANNEL1)
/// @param enable TRUE to enable, FALSE to disable
void drv_timer_interrupt_enable(tmr_type* timer, timer_interrupt_type int_type, confirm_state enable);

/// @brief Get current timer counter value
/// @param timer Timer peripheral to read
/// @return Current counter value
uint32_t drv_timer_get_counter(tmr_type* timer);

/// @brief Set timer counter value
/// @param timer Timer peripheral to configure
/// @param value Counter value to set
void drv_timer_set_counter(tmr_type* timer, uint32_t value);

/// @brief Clear timer interrupt flag
/// @param timer Timer peripheral
/// @param int_type Interrupt type to clear (bit flags, can be OR'ed)
void drv_timer_clear_flag(tmr_type* timer, timer_interrupt_type int_type);

/// @brief Check if timer interrupt flag is set
/// @param timer Timer peripheral
/// @param int_type Interrupt type to check (typically single flag, not combined)
/// @return TRUE if flag is set, FALSE otherwise
confirm_state drv_timer_get_flag(tmr_type* timer, timer_interrupt_type int_type);

/* Legacy function - Initialize timer using tmr_base_init_type */
void tmr_apply_base_init(tmr_type* tmr_x, const tmr_base_init_type* init);

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_DRV_H_ */



#include "TimerDrv.h"

void tmr_apply_base_init(tmr_type* tmr_x, const tmr_base_init_type* init)
{
    if (tmr_x == 0 || init == 0)
    {
        return;
    }

    tmr_base_init(tmr_x, init->period, init->div);
    tmr_cnt_dir_set(tmr_x, (tmr_count_mode_type)init->count_mode);
    tmr_clock_source_div_set(tmr_x, (tmr_clock_division_type)init->clock_division);
    tmr_repetition_counter_set(tmr_x, (uint8_t)init->repetition_counter);
}



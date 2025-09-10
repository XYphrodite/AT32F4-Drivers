#include "rtc_module.h"

// char const weekday_table[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void rtc_module_init(void)
{
    calendar_type time_struct;
    /* config calendar */
    time_struct.year = 2025;
    time_struct.month = 6;
    time_struct.date = 20;
    time_struct.hour = 0;
    time_struct.min = 0;
    time_struct.sec = 0;
    rtc_init(&time_struct);
}

void rtc_module_get_time(calendar_type *ct)
{
    rtc_time_get();

    ct->year = calendar.year;
    ct->month = calendar.month;
    ct->date = calendar.date;
    ct->hour = calendar.hour;
    ct->min = calendar.min;
    ct->sec = calendar.sec;

    /* wait for the register write to complete */
    rtc_wait_config_finish();

    /* clear the rtc second flag */
    rtc_flag_clear(RTC_TS_FLAG);

    /* wait for the register write to complete */
    rtc_wait_config_finish();
}


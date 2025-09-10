#ifndef _RTC_MODULE_H_ 
#define _RTC_MODULE_H_

#include "at32f403a_407.h"

void rtc_module_init(void);
void rtc_module_get_time(calendar_type* ct);

#endif
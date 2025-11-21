#ifndef _WDT_H_
#define _WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Tracing.h"
#include "WatchDogApi.h"
#include "at32f403a_407.h"

void wdt_init(void);
void wdt_try_reload(void);
void wdt_notify_to_subject(WatchDogSubject_t s);

#ifdef __cplusplus
}
#endif

#endif
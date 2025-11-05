#ifndef _WDT_H_
#define _WDT_H_




#ifdef __cplusplus
extern "C" {
#endif

#include "Tracing.h"
#include "at32f403a_407.h"
#include "ErrorJournal.h"


        typedef struct WatchDogSubject {
                bool unit_control : 1;
                bool blinky : 1;
                bool gateway : 1;
                bool uart : 1;
                bool end_node : 1;
                bool temp : 1;
                bool usb : 1;
        } WatchDogSubject_t;

        void wdt_init(void);
        void wdt_try_reload(void);
        void wdt_notify_to_subject(WatchDogSubject_t s);

#ifdef __cplusplus
}
#endif

#endif
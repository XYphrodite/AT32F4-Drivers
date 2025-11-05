#ifndef _SYSTEMAPI_H_
#define _SYSTEMAPI_H_

#include <stdint.h>
#include "McuClock/ClockCfg.h"
#include "UpTime.h"
#include "FlashService.h"
#include "unitControl.h"
#include "UartApi.h"
#include "SimpleWD.h"
#include "BootloadConfig.h"

// System services bitfield - allows selective initialization of MCU services
struct SystemServices
{
    bool UpTimeTmr : 1;
    bool FlashService : 1;
    bool UnitReset : 1;
    bool UartService : 1;
    bool WatchdogTimer : 1;
    bool BootloaderConfig : 1;
    bool reserved : 26;
    usart_init_type* init_config;

    // Constructor to ensure init_config is initialized
    SystemServices() : init_config(nullptr) {}
};

typedef struct SystemServices SystemServices_t;

class SystemApi
{
public:
    SystemApi();
    ~SystemApi();

    static void Init(nvic_priority_group_type priority_group,
    MCU_speed clock,
    bool swd_over_jtag,
    uint32_t vector_table_offset);

    static error_status InitServices(const SystemServices_t& services);
};

#endif // _SYSTEMAPI_H_
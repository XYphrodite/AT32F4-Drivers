/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "at32f403a_407_conf_template.h"
#include "wk_system.h"

#include "at32f403a_407_clock.h"
#include "UartApi.h"
#include "Command.h"
#include "gpio.h"

#ifdef FREE_RTOS_IS_IN_USED
extern "C"
{
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>
}

#include "wdt.h"

#endif

#include "at32f403a_407_usart.h"

void uartFun(void *parameters);

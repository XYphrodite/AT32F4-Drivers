#ifdef LITE_GATEWAY

#include "UartApp.h"

#ifdef BOOTLOADER

void uartFun(void *parameters)
{
    UartService uartService = UartService();
    uint32_t max_time_s = GetUpTimeS() + 600; // ten minutes
    for (uint32_t now = GetUpTimeS(); now - max_time_s; now = GetUpTimeS())
    {
        if (uartService.Read() > 0)
            while (uartService.TryGetCommand())
            {
                uartService.HandleCurrentRxBuff();
                uartService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
            }
#if USE_WDT
        simple_wdt_reload();
#endif
    }
}

#else

void uartFun(void *parameters)
{
    (void)parameters;
    // Initialize USART1 here
    UartService uartService = UartService();
    for (;;)
    {
        if (uartService.Read() > 0)
            while (uartService.TryGetCommand())
            {
                bool handled = false;
                while (!handled)
                {
                    if (xSemaphoreTake(gateway_handled_data_mutex, portMAX_DELAY) == pdTRUE)
                    {
                uartService.HandleCurrentRxBuff();
                        xSemaphoreGive(gateway_handled_data_mutex);
                        handled = true;
                    }
                    else
                    {
                        vTaskDelay(5);
                    }
                }
                uartService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
                CmdHandler.SetResponce(UNKNOWN_CS,NULL, 0);
            }
        wdt_notify_to_subject({.uart = true});
        vTaskDelay(20); // Delay to prevent spamming the transmission
    }
}

#endif

#endif

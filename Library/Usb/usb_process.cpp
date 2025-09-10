

#include "usb_process.h"

UsbService usbService = UsbService();

#if (LITE_GATEWAY && BOOTLOADER) || END_DEVICE_TEMPERATURE_SENSOR

void usb_process(void)
{
    while (usbService.IsConnect() && GetUpTimeS() <= usbService.GetUsbLimitS())
    {
        uint16_t new_data_len = usbService.Read();
        if (new_data_len)
        {
            state_machine.stop_usb = false;
            save_state_machine();
            usbService.AddUsbLimitS(GetUpTimeS());
            if (usbService.TryGetCommand())
            {
                usbService.HandleCurrentRxBuff();
                usbService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
                simple_wdt_reload();
                CmdHandler.DoPostCommand();
                simple_wdt_reload();
                delay_ms(5);
                simple_wdt_reload();
            }
        }
        simple_wdt_reload();
        if (GetUpTimeS() >= 1800)
        {
            return;
        }
    }
    simple_wdt_reload();
    if (GetUpTimeS() >= usbService.GetUsbLimitS())
    {
        state_machine.stop_usb = true;
        save_state_machine();
    }
}

#endif

#ifdef PIGSTORE_PBLOCK

void usb_process(void)
{
}

#endif

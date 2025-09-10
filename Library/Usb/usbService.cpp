#include "usbService.h"

UsbService::UsbService(/* args */)
{
}

UsbService::~UsbService()
{
}

void UsbService::Init(void)
{
    init_usb_drv();
    wk_usb_app_init();
    usb_limit_s = !state_machine.stop_usb ? 600 : 0;
}

bool UsbService::IsConnect(void)
{
    #ifdef HARDWARE_1_0_0_FOR_DEMO
        return false;
    #endif
    if (!isVBUS2_pinInit)
    {
        pinInit(V_BUS_2_PORT, V_BUS_2_PIN, INPUT, GPIO_PULL_NONE);
        isVBUS2_pinInit = true;
    }
    return gpio_input_data_bit_read(V_BUS_2_PORT, V_BUS_2_PIN) == SET;
}

uint32_t UsbService::GetUsbLimitS(void)
{
    return usb_limit_s;
}
void UsbService::AddUsbLimitS(uint32_t add)
{
    usb_limit_s += add;
}

uint32_t all_data_len = 0;

uint16_t UsbService::Read()
{
    data_len = usb_vcp_get_rxdata(&usb_core_dev, usb_buffer);
    all_data_len += data_len;
    if (data_len > 0)
    {
        for (size_t i = 0; i < data_len; i++)
        {
            rx_ring_Buf[rx_buff_head_index] = usb_buffer[i];
            rx_buff_head_index = CUT_OFF(++rx_buff_head_index, USB_RXBUF_SIZE);
        }
    }
    return data_len;
}

bool UsbService::TryGetCommand(void)
{
    while (rx_buff_tail_index != rx_buff_head_index)
    {
        // Проверяем наличие REQUEST_COM_PREFIX
        if (rx_ring_Buf[rx_buff_tail_index] == REQUEST_COM_PREFIX)
        {
            uint16_t prefix_index = rx_buff_tail_index;
            uint16_t code_index = CUT_OFF(prefix_index + 1, USB_RXBUF_SIZE);
            uint8_t code = rx_ring_Buf[code_index];
            uint8_t expectedSize = CmdHandler.GetCommandSize(code);

            if (expectedSize == 0)
            {
                // Пропускаем этот префикс и идём дальше
                rx_buff_tail_index = CUT_OFF(prefix_index + 2, USB_RXBUF_SIZE);
                continue;
            }

            uint16_t available = (rx_buff_head_index + USB_RXBUF_SIZE - prefix_index) % USB_RXBUF_SIZE;
            if (available < expectedSize)
                return false; // недостаточно данных

            // Читаем команду
            uint8_t requestBytes[USB_RXBUF_SIZE] = {0};
            uint16_t temp_index = prefix_index;
            for (uint8_t i = 0; i < expectedSize; ++i)
            {
                requestBytes[i] = rx_ring_Buf[temp_index];
                temp_index = CUT_OFF(temp_index + 1, USB_RXBUF_SIZE);
            }

            // Проверка CRC
            if (count_CRC(requestBytes, expectedSize - 1) != requestBytes[expectedSize - 1])
            {
                rx_buff_tail_index = CUT_OFF(prefix_index + 1, USB_RXBUF_SIZE);
                continue;
            }

            memcpy(com_buffer, requestBytes, expectedSize);
            this->comSize = expectedSize;
            rx_buff_tail_index = temp_index; // Принятая команда
            return true;
        }

        // Не префикс — просто двигаем указатель
        rx_buff_tail_index = CUT_OFF(rx_buff_tail_index + 1, USB_RXBUF_SIZE);
    }

    return false;
}

void UsbService::HandleCurrentRxBuff(void)
{
    CmdHandler.Do(com_buffer);
}

bool UsbService::Send(const uint8_t *buffer, uint8_t len)
{
    PacketWrapper::Wrap(buffer, len, usb_buffer, &data_len, CmdHandler.GetLastComCode());
    usb_vcp_send_data(&usb_core_dev, usb_buffer, data_len);
    return true;
}
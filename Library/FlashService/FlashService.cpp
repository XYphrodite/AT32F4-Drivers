#include "FlashService.h"

void FlashService::Init(void)
{
    flash_unlock();

    flash_flag_clear(FLASH_BANK1_OBF_FLAG);
    flash_flag_clear(FLASH_BANK1_ODF_FLAG);
    flash_flag_clear(FLASH_BANK1_PRGMERR_FLAG);
    flash_flag_clear(FLASH_BANK1_EPPERR_FLAG);
    flash_flag_clear(FLASH_BANK2_OBF_FLAG);
    flash_flag_clear(FLASH_BANK2_ODF_FLAG);
    flash_flag_clear(FLASH_BANK2_PRGMERR_FLAG);
    flash_flag_clear(FLASH_BANK2_EPPERR_FLAG);
    flash_flag_clear(FLASH_USDERR_FLAG);
}

void FlashService::WaitWhileBusy(void)
{
    while (flash_flag_get(FLASH_BANK1_OBF_FLAG) == SET || flash_flag_get(FLASH_BANK2_OBF_FLAG) == SET)
        ;
}

bool FlashService::EraseSectorByNum(uint16_t sector_num)
{
    WaitWhileBusy();
    flash_status_type result = flash_sector_erase(FLASH_MEMORY_START + SECTOR_SIZE * sector_num);
#ifdef USE_WDT
    simple_wdt_reload();
#endif
    return FLASH_OPERATE_DONE == result;
}

bool FlashService::EraseSectorByAddr(uint32_t address)
{
    WaitWhileBusy();
#ifdef USE_WDT
    simple_wdt_reload();
#endif
    return flash_sector_erase(address) == FLASH_OPERATE_DONE;
}

bool FlashService::EraseSectorByAddr(uint32_t address, uint16_t amount)
{
    while (amount--)
    {
        FlashService::EraseSectorByAddr(address + amount * SECTOR_SIZE);
    }
    return true;
}

bool FlashService::Programm(uint32_t address, uint8_t *buff, uint32_t length, block_size size)
{
    bool result = true;
    for (size_t i = 0; i < length;)
    {
        WaitWhileBusy();
        switch (size)
        {
        case byte:
            result = flash_byte_program(address, *buff) == FLASH_OPERATE_DONE;
            address += 1;
            buff += 1;
            i += 1;
            break;
        case halfword:
            result = flash_halfword_program(address, *reinterpret_cast<uint16_t *>(buff)) == FLASH_OPERATE_DONE;
            address += 2;
            buff += 2;
            i += 2;
            break;
        case word:
            result = flash_word_program(address, *reinterpret_cast<uint32_t *>(buff)) == FLASH_OPERATE_DONE;
            address += 4;
            buff += 4;
            i += 4;
            break;
        default:
            break;
        }
        if (!result)
            break;
    }

    return result;
}

bool FlashService::Programm(uint32_t address, uint8_t *buff, uint32_t length)
{
    bool result = true;
    for (size_t i = 0; i < length;)
    {
        WaitWhileBusy();
        if (length - i >= 4)
        {
            result = flash_word_program(address, *reinterpret_cast<uint32_t *>(buff)) == FLASH_OPERATE_DONE;
            address += 4;
            buff += 4;
            i += 4;
        }
        else if (length - i >= 4)
        {
            result = flash_halfword_program(address, *reinterpret_cast<uint16_t *>(buff)) == FLASH_OPERATE_DONE;
            address += 2;
            buff += 2;
            i += 2;
        }
        else
        {
            result = flash_byte_program(address, *buff) == FLASH_OPERATE_DONE;
            address += 1;
            buff += 1;
            i += 1;
        }
        if (!result)
            break;
    }

    return result;
}

#include <cstring> // for memcpy

bool FlashService::Read(uint32_t address, void *buff, uint32_t size)
{
    uint8_t *buff_ptr = reinterpret_cast<uint8_t *>(buff);
    for (size_t i = 0; i < size;)
    {
        if (size - i >= 4)
        {
            uint32_t value = *reinterpret_cast<uint32_t *>(address);
            std::memcpy(buff_ptr, &value, sizeof(uint32_t));
            address += sizeof(uint32_t);
            buff_ptr += sizeof(uint32_t);
            i += sizeof(uint32_t);
        }
        else if (size - i >= 2)
        {
            uint16_t value = *reinterpret_cast<uint16_t *>(address);
            std::memcpy(buff_ptr, &value, sizeof(uint16_t));
            address += sizeof(uint16_t);
            buff_ptr += sizeof(uint16_t);
            i += sizeof(uint16_t);
        }
        else
        {
            *buff_ptr = *reinterpret_cast<uint8_t *>(address);
            address += sizeof(uint8_t);
            buff_ptr += sizeof(uint8_t);
            i += sizeof(uint8_t);
        }
    }

    return true;
}

bool FlashService::IsDiffer(uint32_t address, void *buff, uint32_t size)
{
    uint8_t *flash_ptr = reinterpret_cast<uint8_t *>(address);
    uint8_t *buff_ptr = reinterpret_cast<uint8_t *>(buff);

    for (uint32_t i = 0; i < size; ++i)
        if (flash_ptr[i] != buff_ptr[i])
            return true;
    return false;
}

bool FlashService::Reprogramm(uint32_t address, void *buff, uint32_t size)
{
    if (FlashService::EraseSectorByAddr(address))
    {
        delay_ms(50);
        return FlashService::Programm(address, reinterpret_cast<uint8_t *>(buff), size);
    }
    return false;
}

bool FlashService::CheckDiffAndReprogramm(uint32_t address, void *buff, uint32_t size)
{
    if (FlashService::IsDiffer(address, buff, size))
        return FlashService::Reprogramm(address, buff, size);
    return true;
}

bool FlashService::IsMemoryProtect(void)
{
    return flash_fap_status_get() == SET;
}

bool FlashService::SetMemoryProtect(bool new_state)
{
    if (new_state)
    {
        if (IsMemoryProtect())
            return true;
        else
            return FLASH_OPERATE_DONE == flash_fap_enable(TRUE);
    }
    else
    {
        if (IsMemoryProtect())
            return FLASH_OPERATE_DONE == flash_fap_enable(FALSE);
        else
            return true;
    }
}

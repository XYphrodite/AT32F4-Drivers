#include "BootloadConfig.h"

#ifndef PREBOOT

uint8_t BootloadConfig::config_version;

version_t BootloadConfig::hardware_version;
version_t BootloadConfig::bootloader_version;

uint32_t BootloadConfig::jump_address;
uint32_t BootloadConfig::max_app_size;

uint32_t BootloadConfig::reserve_app_address;
version_t BootloadConfig::reserve_app_version;
uint32_t BootloadConfig::reserve_app_length;

#ifdef END_DEVICE_TEMPERATURE_SENSOR
uint32_t BootloadConfig::fuota_app_address;
version_t BootloadConfig::fuota_app_version;
uint32_t BootloadConfig::fuota_app_length;
#endif

bootloader_flags_t BootloadConfig::bootloader_flags;

#ifdef LITE_GATEWAY
LbsFlags BootloadConfig::lbs_flags;
#endif

uint8_t BootloadConfig::crc;

bool BootloadConfig::isOkay; // doesn't save to flash

uint8_t BootloadConfig::buffer[100];

void BootloadConfig::Init(void)
{
    LoadConfig();
    isOkay = CheckCRC() && config_version == BOOTLOAD_CONFIG_VERSION;
    if (!isOkay)
    {
        SetDefault();

        bootloader_flags.is_main_app_ok = CheckMainAppExistence();
        bootloader_flags.is_reserv_app_ok = CheckFirmwareExistence(reserve_app_address, reserve_app_length);
#ifdef END_DEVICE_TEMPERATURE_SENSOR
        bootloader_flags.is_fuota_app_ok = CheckFirmwareExistence(fuota_app_address, fuota_app_length);
#endif

        SetCrcAndSave();
        isOkay = true;
    }
}

void BootloadConfig::LoadConfig(void)
{
    uint32_t start_addr = ADDRESS_BOOTLOAD_CONGIG;
    SerialReadFromFlash(start_addr, config_version);
    SerialReadFromFlash(start_addr, hardware_version);
    SerialReadFromFlash(start_addr, bootloader_version);
    SerialReadFromFlash(start_addr, jump_address);
    SerialReadFromFlash(start_addr, max_app_size);

    // -- reserve firmware --
    SerialReadFromFlash(start_addr, reserve_app_address);
    SerialReadFromFlash(start_addr, reserve_app_version);
    SerialReadFromFlash(start_addr, reserve_app_length);

#ifdef END_DEVICE_TEMPERATURE_SENSOR
    // -- fuota firmware --
    SerialReadFromFlash(start_addr, fuota_app_address);
    SerialReadFromFlash(start_addr, fuota_app_version);
    SerialReadFromFlash(start_addr, fuota_app_length);
#endif

    SerialReadFromFlash(start_addr, bootloader_flags);

#ifdef LITE_GATEWAY
    SerialReadFromFlash(start_addr, lbs_flags);
#endif

    SerialReadFromFlash(start_addr, crc);
}

void BootloadConfig::SaveConfig(void)
{
    size_t size = GetSize();
    Serialize(buffer, size);
    FlashService::CheckDiffAndReprogramm(ADDRESS_BOOTLOAD_CONGIG, buffer, size);
}

bool BootloadConfig::CheckCRC(void)
{
    return CountCRC() == crc;
}

/// @brief Set default values to modem config
/// @param
void BootloadConfig::SetDefault(void)
{
    uint32_t hardware_version_uint32 = HARDWARE_VERSION;
    version_t _hardware_version = *reinterpret_cast<version_t *>(&hardware_version_uint32);
    uint32_t bootloader_version_uint32 = BOOTLOADER_VERSION;
    version_t _bootloader_version = *reinterpret_cast<version_t *>(&bootloader_version_uint32);

    config_version = BOOTLOAD_CONFIG_VERSION;
    hardware_version = _hardware_version;
    bootloader_version = _bootloader_version;

    jump_address = DEFAULT_MAIN_APP_ADDRESS;
    max_app_size = DEFAULT_MAX_APP_SIZE;

    reserve_app_address = DEFAULT_RESERVE_APP_ADDRESS;
    reserve_app_version = DEFAULT_RESERVE_APP_VERSION;
    reserve_app_length = DEFAULT_RESERVE_APP_LENGTH;
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    fuota_app_address = DEFAULT_FUOTA_APP_ADDRESS;
    fuota_app_version = DEFAULT_FUOTA_APP_VERSION;
    fuota_app_length = DEFAULT_FUOTA_APP_LENGTH;
#endif

    bootloader_flags = DEFAULT_BOOTLOADER_FLAGS;

#ifdef LITE_GATEWAY
    lbs_flags = DEFAULT_LBS_FLAGS;
#endif

    SetCRC();
}

void BootloadConfig::SetJumpAddress(uint32_t address)
{
    if (jump_address != address)
    {
        jump_address = address;
        SetCrcAndSave();
    }
}

uint32_t BootloadConfig::GetJumpAddress(void)
{
    return isOkay ? jump_address : DEFAULT_MAIN_APP_ADDRESS;
}

void BootloadConfig::SetMaxAppSize(uint32_t app_size)
{
    if (max_app_size != app_size)
    {
        max_app_size = app_size;
        SetCrcAndSave();
    }
}

uint32_t BootloadConfig::GetMaxAppSize(void)
{
    return isOkay ? max_app_size : DEFAULT_MAX_APP_SIZE;
}

version_t BootloadConfig::GetHardwareVersion(void)
{
    uint32_t hardware_version_uint32 = HARDWARE_VERSION;
    version_t version = *reinterpret_cast<version_t *>(&hardware_version_uint32);
    return isOkay ? hardware_version : version;
}

void BootloadConfig::SetHardwareVersion(version_t version)
{
    if (memcmp(&version, &hardware_version, sizeof(version)))
    {
        hardware_version = version;
        SetCrcAndSave();
    }
}

version_t BootloadConfig::GetBootloaderVersion(void)
{
    uint32_t bootloader_version_uint32 = BOOTLOADER_VERSION;
    version_t version = *reinterpret_cast<version_t *>(&bootloader_version_uint32);
    return isOkay ? bootloader_version : version;
}

/// @brief count current crc value and set it to ModemConfig
/// @param
void BootloadConfig::SetCRC(void)
{
    crc = CountCRC();
}

/// @brief count crc
/// @param
/// @return crc as uint8_t
uint8_t BootloadConfig::CountCRC(void)
{
    size_t size = GetSize();
    uint8_t buff[size];
    Serialize(buff, size);

    return count_CRC(buff, size - sizeof(crc));
}

// Serialize function
void BootloadConfig::Serialize(uint8_t *buffer, size_t &size)
{
    size = GetSize();
    uint8_t *cursor = buffer;

    WriteToBuffer(cursor, config_version);
    WriteToBuffer(cursor, hardware_version);
    WriteToBuffer(cursor, bootloader_version);
    WriteToBuffer(cursor, jump_address);
    WriteToBuffer(cursor, max_app_size);

    // -- reserve firmware --
    WriteToBuffer(cursor, reserve_app_address);
    WriteToBuffer(cursor, reserve_app_version);
    WriteToBuffer(cursor, reserve_app_length);

#ifdef END_DEVICE_TEMPERATURE_SENSOR
    // -- fuota firmware --
    WriteToBuffer(cursor, fuota_app_address);
    WriteToBuffer(cursor, fuota_app_version);
    WriteToBuffer(cursor, fuota_app_length);
#endif

    WriteToBuffer(cursor, bootloader_flags);

#ifdef LITE_GATEWAY
    WriteToBuffer(cursor, lbs_flags);
#endif
    WriteToBuffer(cursor, crc);
}

size_t BootloadConfig::GetSize(void)
{
    return sizeof(config_version) +
           sizeof(hardware_version) +
           sizeof(bootloader_version) +
           sizeof(jump_address) +
           sizeof(max_app_size) +
           /*reserve*/
           sizeof(reserve_app_address) +
           sizeof(reserve_app_version) +
           sizeof(reserve_app_length) +
    /*-------*/
#ifdef END_DEVICE_TEMPERATURE_SENSOR
           /*fuota*/
           sizeof(fuota_app_address) +
           sizeof(fuota_app_version) +
           sizeof(fuota_app_length) +
    /*-----*/
#endif
           sizeof(bootloader_flags) +
#ifdef LITE_GATEWAY
           sizeof(lbs_flags) +
#endif
           sizeof(crc);
}

bootloader_flags_t BootloadConfig::GetBootloaderFlags(void)
{
    return isOkay ? bootloader_flags : DEFAULT_BOOTLOADER_FLAGS;
}

void BootloadConfig::SetBootloaderFlags(bootloader_flags_t new_flags)
{
    if (memcmp(&bootloader_flags, &new_flags, sizeof(bootloader_flags)) != 0)
    {
        bootloader_flags = new_flags;
        SetCrcAndSave();
    }
}

#ifdef LITE_GATEWAY
void BootloadConfig::SetLbsFlags(LbsFlags new_flags)
{
    if (memcmp(&lbs_flags, &new_flags, sizeof(lbs_flags)) != 0)
    {
        lbs_flags = new_flags;
        SetCrcAndSave();
    }
}
LbsFlags BootloadConfig::GetLbsFlags(void)
{
    return isOkay ? lbs_flags : DEFAULT_LBS_FLAGS;
}
#endif

void BootloadConfig::SetCrcAndSave(void)
{
    SetCRC();
    SaveConfig();
}

bool BootloadConfig::RestoreFirmware(other_firmware fw)
{
    uint32_t read_addr = 0;
    uint32_t write_addr = jump_address;
    uint32_t erase_length = max_app_size; // kb
    uint32_t write_length = 0;            // b
    switch (fw)
    {
    case reserveFW:
        read_addr = reserve_app_address;
        write_length = reserve_app_length;
        break;
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    case fuotaFW:
        read_addr = fuota_app_address;
        write_length = fuota_app_length;
        break;
#endif
    default:
        return false;
    }
    // check firmware existence
    bool isExist = CheckFirmwareExistence(read_addr, write_length);
    if (!isExist)
        return false;

    // erase main app area
    bool result = FlashService::EraseSectorByAddr(write_addr, erase_length / 2);

    for (size_t i = 0; i < write_length; i++)
    {
        uint8_t data = *reinterpret_cast<uint8_t *>(read_addr);
        result &= FlashService::Programm(write_addr, &data, 1);
        read_addr += 1;
        write_addr += 1;
        if (!result)
            break;
    }

    return result;
}

bool BootloadConfig::CheckFirmwareExistence(uint32_t addr, uint32_t length)
{
    // Check address alignment
    if (addr % 4 != 0 || length % 4 != 0)
    {
        // Handle alignment error (return false, or handle differently)
        return false;
    }

    volatile uint32_t *flash_ptr = reinterpret_cast<volatile uint32_t *>(addr);
    uint32_t word_count = length / 4;
    word_count = word_count >= 512 ? 512 : word_count;

    for (size_t i = 0; i < word_count; ++i)
    {
        if (flash_ptr[i] != 0xFFFFFFFF)
        {
            return true; // Firmware data exists
        }
    }

    return false; // All erased
}

bool BootloadConfig::CheckMainAppExistence(void)
{
    return CheckFirmwareExistence(jump_address, max_app_size * 1024);
}

void BootloadConfig::HandleBootFlags()
{
    // allback firmware
    if (bootloader_flags.need_reprogramm_from_reserv)
    {
        if (BootloadConfig::RestoreFirmware(reserveFW))
        {
            bootloader_flags.is_reserv_using = true;
            bootloader_flags.is_main_app_ok = true;
        }
        else
            bootloader_flags.is_reserv_app_ok = false;
        bootloader_flags.need_reprogramm_from_reserv = false;
        reset_failed_booting();
        BootloadConfig::SetBootloaderFlags(bootloader_flags);
        mcu_safe_reset();
    }

    if (bootloader_flags.need_reprogramm_from_fuota)
    {
        if (BootloadConfig::RestoreFirmware(fuotaFW))
        {
            bootloader_flags.is_fuota_using = true;
            bootloader_flags.is_main_app_ok = true;
        }
        else
            bootloader_flags.is_fuota_app_ok = false;
        bootloader_flags.need_reprogramm_from_fuota = false;
        reset_failed_booting();
        BootloadConfig::SetBootloaderFlags(bootloader_flags);
        mcu_safe_reset();
    }

    // check failed bootings
    if (service_data.failed_booting >= MAX_FAILED_BOOTING || !BootloadConfig::CheckMainAppExistence())
    {
        if (!HasAnyOkFw())
            return;
        bootloader_flags.is_main_app_ok = false;
        if (bootloader_flags.is_reserv_using)
            bootloader_flags.is_reserv_app_ok = false;
        if (bootloader_flags.is_fuota_using)
            bootloader_flags.is_fuota_app_ok = false;

        reset_failed_booting();
        // restore firmware
        if (bootloader_flags.is_reserv_app_ok)
            bootloader_flags.need_reprogramm_from_reserv = true;
        else if (bootloader_flags.is_fuota_app_ok)
            bootloader_flags.need_reprogramm_from_fuota = true;

        BootloadConfig::SetBootloaderFlags(bootloader_flags);
        mcu_safe_reset();
    }
}

bool BootloadConfig::CanRunMainApp(void)
{
    return bootloader_flags.is_main_app_ok && BootloadConfig::CheckMainAppExistence();
}

bool BootloadConfig::HasAnyOkFw(void)
{
    return bootloader_flags.is_main_app_ok || bootloader_flags.is_reserv_app_ok || bootloader_flags.is_fuota_app_ok;
}

#endif

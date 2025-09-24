#ifndef _BOOTLOAD_CONFIG_H_
#define _BOOTLOAD_CONFIG_H_

#ifndef PREBOOT

#include <at32f403a_407.h>
#include "FlashService.h"
#include "CRC.h"
#include "main_defines.h"
#include "FW_Loader.h"
#include "Structs.h"
#include "Shared.h"
#include "mcu_hal.h"
#include "version_define.h"

#pragma pack(push, 1)

enum other_firmware
{
    unknownFW = 0x0U,
    reserveFW = 0x1U,
    fuotaFW = 0x2U,
};

typedef struct bootloader_flags_e
{
    bool is_main_app_ok : 1;
    bool is_reserv_app_ok : 1;
    bool is_fuota_app_ok : 1;
    bool need_reprogramm_from_reserv : 1;
    bool need_reprogramm_from_fuota : 1;
    bool is_reserv_using : 1;
    bool is_fuota_using : 1;
    uint16_t reserv : 9;
} bootloader_flags_t;
#pragma pack(pop)

#define SEC_BOOTLOAD_CONGIG (255U)
#define ADDRESS_BOOTLOAD_CONGIG (SECTOR_ADDRESS(SEC_BOOTLOAD_CONGIG))
#define BOOTLOAD_CONFIG_VERSION (1U)

#define DEFAULT_MAIN_APP_ADDRESS (0x8009000U)
#define DEFAULT_MAX_APP_SIZE (256U) // kb

#define DEFAULT_RESERVE_APP_SECTOR (384U)
#define DEFAULT_RESERVE_APP_ADDRESS (SECTOR_ADDRESS(DEFAULT_RESERVE_APP_SECTOR))
const version_t DEFAULT_RESERVE_APP_VERSION{0U, 0U, 0U, (version_type)0U};
#define DEFAULT_RESERVE_APP_LENGTH (0U)

#define DEFAULT_FUOTA_APP_SECTOR (384U)
#define DEFAULT_FUOTA_APP_ADDRESS (SECTOR_ADDRESS(DEFAULT_FUOTA_APP_SECTOR))
const version_t DEFAULT_FUOTA_APP_VERSION{0U, 0U, 0U, (version_type)0U};
#define DEFAULT_FUOTA_APP_LENGTH (0U)

const bootloader_flags_t DEFAULT_BOOTLOADER_FLAGS{true, false, false, false, false, false, false, 0U};

#define DEFAULT_LBS_FLAGS ((LbsFlags)lbs_unknown_board)

#define MAX_FAILED_BOOTING (10U)

struct BootloadConfig
{
private:
    static uint8_t config_version;
    static version_t hardware_version;
    static version_t bootloader_version;
    static uint32_t jump_address;
    static uint32_t max_app_size;
    static uint32_t reserve_app_address;
    static version_t reserve_app_version;
    static uint32_t reserve_app_length;
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    static uint32_t fuota_app_address;
    static version_t fuota_app_version;
    static uint32_t fuota_app_length;
#endif
    static bootloader_flags_t bootloader_flags;
    static LbsFlags lbs_flags;

    static uint8_t crc;

    static uint8_t buffer[100];

    static bool CheckCRC(void);
    static void SetDefault(void);
    static void SetCRC(void);
    static uint8_t CountCRC(void);
    static void Serialize(uint8_t *buffer, size_t &size);
    static size_t GetSize(void);

    static void LoadConfig(void);
    static void SaveConfig(void);

    static bool isOkay; // doesn't save to flash

    BootloadConfig(/* args */);
    static void SetCrcAndSave(void);

public:
    // Deleting the copy constructor to prevent copies
    BootloadConfig(const BootloadConfig &obj) = delete;
    // BootloadConfig& operator=(const BootloadConfig&) = delete;

    ~BootloadConfig() {}

    static void Init(void);

    static uint8_t GetConfigVersion(void);
    static version_t GetHardwareVersion(void);
    static void SetHardwareVersion(version_t version);
    static version_t GetBootloaderVersion(void);

    static void SetJumpAddress(uint32_t address);
    static uint32_t GetJumpAddress(void);

    static void SetMaxAppSize(uint32_t app_size);
    static uint32_t GetMaxAppSize(void);

    /*reserve firmware------------*/
    static uint32_t GetReserveAppAddress(void);
    static void SetReserveAppAddress(uint32_t address);
    static version_t GetReserveAppVersion(void);
    static void SetReserveAppVersion(version_t version);
    static uint32_t GetReserveAppLength(void);
    static void SetReserveAppLength(uint32_t);
    /*----------------------------*/

    /*fuota firmware--------------*/
    static uint32_t GetFuotaAppAddress(void);
    static void SetFuotaAppAddress(uint32_t address);
    static version_t GetFuotaAppVersion(void);
    static void SetFuotaAppVersion(version_t version);
    static uint32_t GetFuotaAppLength(void);
    static void SetFuotaAppLength(uint32_t);
    /*----------------------------*/

    static bootloader_flags_t GetBootloaderFlags(void);
    static void SetBootloaderFlags(bootloader_flags_t new_flags);

    static void SetLbsFlags(LbsFlags new_flags);
    static LbsFlags GetLbsFlags(void);

    static bool RestoreFirmware(other_firmware fw);
    static bool CheckFirmwareExistence(uint32_t addr, uint32_t length);
    static bool CheckMainAppExistence(void);

    static void HandleBootFlags(void);
    static bool CanRunMainApp(void);

    static bool HasAnyOkFw(void);
};
// -----------------------
// FlashMap
// -----------------------
// config_version       1b
// hardware_version     4b
// bootloader_version   4b
// jump_address         4b
// max_app_size         4b
// reserve_app_address  4b
// reserve_app_version  4b
// reserve_app_length   4b
//--only end device section--
// fuota_app_address    4b
// fuota_app_version    4b
// fuota_app_length     4b
//---------------------------
// bootloader_flags     2b
// crc                  1b  crc must be last
// -----------------------

#endif

#endif

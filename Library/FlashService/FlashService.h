#ifndef _FLASH_SERVICE_
#define _FLASH_SERVICE_

#include "at32f403a_407.h"
#include <stdbool.h>
#include <string.h>
#include "DelayService.h"
#if (END_DEVICE_TEMPERATURE_SENSOR || BOOTLOADER)
#include "SimpleWD.h"
#endif

#define FLASH_MEMORY_START (FLASH_BANK1_START_ADDR)
#define FLASH_MEMORY_SIZE_KB (1024U) // kb
#define SECTOR_SIZE (2 * 1024)       // bytes
#define SECTOR_NUMBER (512U)         //

#define SECTOR_ADDRESS(num) FLASH_MEMORY_START + SECTOR_SIZE *num

class FlashService
{
public:
    enum block_size
    {
        unknown_block_size = 0U,
        byte = 1U,
        halfword = 2U,
        word = 4U
    };

private:
    /* data */

    FlashService(/* args */);
    static void WaitWhileBusy(void);
    static bool Programm(uint32_t address, uint8_t *buff, uint32_t length, block_size size);

public:
    ~FlashService() = delete;

    static void Init(void);
    // read
    static bool Read(uint32_t address, uint8_t *buff, uint32_t size);
    // erase
    static bool EraseSectorByNum(uint16_t sector_num);
    static bool EraseSectorByAddr(uint32_t address);
    static bool EraseSectorByAddr(uint32_t address, uint16_t amount);

    // utilities
    static bool IsDiffer(uint32_t address, uint8_t *buff, uint32_t size);

    // programm
    static bool Programm(uint32_t address, uint8_t *buff, uint32_t length);
    static bool Reprogramm(uint32_t address, uint8_t *buff, uint32_t size);
    static bool CheckDiffAndReprogramm(uint32_t address, uint8_t *buff, uint32_t size);

    // memory protection
    static bool IsMemoryProtect(void);
    static bool SetMemoryProtect(bool new_state);
};

bool fs_erase_sectors_by_adr(uint32_t address, uint16_t amount);

#endif
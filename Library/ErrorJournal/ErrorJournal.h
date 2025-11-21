#ifndef _ERROR_JOURNAL_H_
#define _ERROR_JOURNAL_H_

#include "at32f403a_407.h"
#include "FlashService.h"
#include <cstdint>
#include <string.h>
#include "CRC.h"

// Include project-specific flash memory layout if available
// Each project (P-block, bootloader0, prebootload) should define flash_map.h
// in their Main/inc/ directory
#ifdef __has_include
    #if __has_include("flash_map.h")
        #include "flash_map.h"
    #else
        // Fallback if flash_map.h is not available
        #define ERROR_JOURNAL_ADDRESS (0x08000000U + (243 * 2048U))  // Default: sector 243
    #endif
#else
    // For compilers without __has_include support
    #define ERROR_JOURNAL_ADDRESS (0x08000000U + (243 * 2048U))
#endif

typedef struct ErrorJournalCounts {
  uint32_t wdt_rst;
  uint32_t spi1_rst;
  uint32_t uart1_rst;
  uint32_t lora_rst;
  uint32_t mcu_rst;
  uint8_t crc;
} ErrorJournalCounts_t;

class ErrorJournal {
private:
  static ErrorJournalCounts_t errJrnlCounts;

  static bool LoadData(void);

public:
  ErrorJournal(void) = delete;
  ~ErrorJournal(void) = delete;

  static void Init(void);
  static void SaveData(void);
  static void CheckWDTRst(void);
};

#endif

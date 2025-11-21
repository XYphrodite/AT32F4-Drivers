#ifndef _ERROR_JOURNAL_H_
#define _ERROR_JOURNAL_H_

#include "at32f403a_407.h"
#include "FlashService.h"
#include "flash_map.h"
#include <cstdint>
#include <string.h>
#include "CRC.h"

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

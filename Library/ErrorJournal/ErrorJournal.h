#ifndef _ERROR_JOURNAL_H_
#define _ERROR_JOURNAL_H_

#include "CRC.h"
#include "FlashService.h"
#include "at32f403a_407.h"
#include "rtc.h"
#include <cstdint>
#include <string.h>

// Include project-specific flash memory layout if available
// Each project (P-block, bootloader0, prebootload) should define flash_map.h
// in their Main/inc/ directory
#ifdef __has_include
#if __has_include("flash_map.h")
#include "flash_map.h"
#else
// Fallback if flash_map.h is not available
#define ERROR_JOURNAL_ADDRESS                                                  \
  (0x08000000U + (243 * 2048U)) // Default: sector 243
#endif
#else
// For compilers without __has_include support
#define ERROR_JOURNAL_ADDRESS (0x08000000U + (243 * 2048U))
#endif

// typedef struct ErrorJournalCounts {
//   uint32_t wdt_rst;
//   uint32_t spi1_rst;
//   uint32_t uart1_rst;
//   uint32_t lora_rst;
//   uint32_t mcu_rst;
//   uint8_t crc;
// } ErrorJournalCounts_t;

enum ErrorJournalRecordType : uint8_t {
  ERROR_TYPE_WDT = 0,      // Watchdog timeout
  ERROR_TYPE_SPI1,         // SPI1 communication error
  ERROR_TYPE_UART1,        // UART1 communication error
  ERROR_TYPE_CAN,          // CAN bus error
  ERROR_TYPE_MCU,          // MCU internal error
  ERROR_TYPE_MODBUS,       // Modbus protocol error
  ERROR_TYPE_ADC,          // ADC error
  ERROR_TYPE_GPIO,         // GPIO error
  ERROR_TYPE_COIL,         // Relay/Coil control error
  ERROR_TYPE_CONFIG,       // Configuration error
  ERROR_TYPE_USER,         // User-defined error
  ERROR_TYPE_TEMPERATURE_SENSOR, // Temperature sensor error
  ERROR_TYPE_MAX
};

typedef struct {
  ErrorJournalRecordType type;
  uint8_t data_bytes_cnt;
} ErrorJournalRecordMap_t;



typedef struct ErrorJournalRecord {
  ErrorJournalRecordType type;              // Тип ошибки (ErrorJournalRecordType)
  uint8_t level;             // Уровень: 0=trace, 1=warning, 2=error, 3=fatal
  uint32_t timestamp;        // Время возникновения (RTC или uptime в секундах)
  // uint16_t code;             // Код ошибки (зависит от типа)
  // uint16_t context;          // Контекст (например, task ID, register address)
  char message[26];          // Краткое сообщение (опционально)
} ErrorJournalRecord_t;


class ErrorJournal {
private:
  static ErrorJournalRecord_t errJrnlCountsRecord[1280];

  static bool LoadData(void);

public:
  ErrorJournal(void) = delete;
  ~ErrorJournal(void) = delete;

  static void Init(void);
  static void SaveData(void);
  static void CheckWDTRst(void);
};

#endif

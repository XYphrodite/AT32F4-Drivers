#ifndef _ERROR_JOURNAL_H_
#define _ERROR_JOURNAL_H_

#include "CRC.h"
#include "FlashService.h"
#include "at32f403a_407.h"
#include "rtc.h"
#include <cstdint>
#include <cstring>

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
  ERROR_TYPE_UNKNOWN = 0,        // Unknown error
  ERROR_TYPE_WDT,                // Watchdog timeout
  ERROR_TYPE_SPI1,               // SPI1 communication error
  ERROR_TYPE_UART1,              // UART1 communication error
  ERROR_TYPE_CAN,                // CAN bus error
  ERROR_TYPE_MCU,                // MCU internal error
  ERROR_TYPE_MODBUS,             // Modbus protocol error
  ERROR_TYPE_ADC,                // ADC error
  ERROR_TYPE_GPIO,               // GPIO error
  ERROR_TYPE_COIL,               // Relay/Coil control error
  ERROR_TYPE_CONFIG,             // Configuration error
  ERROR_TYPE_USER,               // User-defined error
  ERROR_TYPE_TEMPERATURE_SENSOR, // Temperature sensor error
  ERROR_TYPE_MAX
};

enum ErrorLevelType : uint8_t {
  ERROR_LEVEL_UNKNOWN = 0, // Unknown level
  ERROR_LEVEL_TRACE = 1,   // Trace level
  ERROR_LEVEL_WARNING = 2, // Warning level
  ERROR_LEVEL_ERROR = 3,   // Error level
  ERROR_LEVEL_FATAL = 4,   // Fatal level
  ERROR_LEVEL_MAX
};

typedef struct {
  ErrorJournalRecordType type;
  uint8_t data_bytes_cnt;
} ErrorJournalRecordMap_t;

class ErrorJournalRecord {
private:
  uint16_t index;
  uint32_t address;
  ErrorJournalRecordType type; // Тип ошибки (ErrorJournalRecordType)
  ErrorLevelType level;        // Уровень ошибки (ErrorLevelType)
  uint32_t timestamp; // Время возникновения (RTC или uptime в секундах)
  // uint16_t code;             // Код ошибки (зависит от типа)
  // uint16_t context;          // Контекст (например, task ID, register
  // address)
  char _message[26]; // Краткое сообщение (опционально)

public:
  ErrorJournalRecord(const ErrorJournalRecordType type = ERROR_TYPE_UNKNOWN,
                     const ErrorLevelType level = ERROR_LEVEL_UNKNOWN,
                     const char* message = "",
                     const bool saving = false);
  void LoadFromFlashByAdress(uint32_t address);
  void LoadFromFlashByIndex(uint16_t index);
};

class ErrorJournal {
private:
  uint16_t total_records; // Общее количество записей
  uint16_t last_index;    // Индекс последней записи

  // static ErrorJournalRecord errJrnlCountsRecord[1280]; # TOO MUCH MEMORY USING

  static bool LoadData(void);
  static void SaveData(void);

  static void CheckWDTRst(void);

public:
  ErrorJournal(void) = delete;
  ~ErrorJournal(void) = delete;

  static void Init(void);



  static constexpr ErrorJournalRecordMap_t ErrorJournalRecordMap[] = {
    {ERROR_TYPE_UNKNOWN, 0},            // Unknown error
    {ERROR_TYPE_WDT, 0},                // Watchdog timeout
    {ERROR_TYPE_SPI1, 0},               // SPI1 communication error
    {ERROR_TYPE_UART1, 0},              // UART1 communication error
    {ERROR_TYPE_CAN, 0},                // CAN bus error
    {ERROR_TYPE_MCU, 0},                // MCU internal error
    {ERROR_TYPE_MODBUS, 0},             // Modbus protocol error
    {ERROR_TYPE_ADC, 0},                // ADC error
    {ERROR_TYPE_GPIO, 0},               // GPIO error
    {ERROR_TYPE_COIL, 0},               // Relay/Coil control error
    {ERROR_TYPE_CONFIG, 0},             // Configuration error
    {ERROR_TYPE_USER, 0},               // User-defined error
    {ERROR_TYPE_TEMPERATURE_SENSOR, 0}, // Temperature sensor error
  };
  static const uint16_t ErrorJournalRecordMapSize = sizeof(ErrorJournalRecordMap) / sizeof(ErrorJournalRecordMap[0]);

  static ErrorJournalRecordMap_t GetErrMap(ErrorJournalRecordType type);
};

#endif

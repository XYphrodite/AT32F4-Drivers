#include "ErrorJournal.h"
#include "FlashService.h"

ErrorJournalRecordMap_t ErrorJournalRecordMap[] = {
    {ERROR_TYPE_WDT, 0},                          // Watchdog timeout
    {ERROR_TYPE_SPI1, 0},                         // SPI1 communication error
    {ERROR_TYPE_UART1, 0},                        // UART1 communication error
    {ERROR_TYPE_CAN, 0},                          // CAN bus error
    {ERROR_TYPE_MCU, 0},                          // MCU internal error
    {ERROR_TYPE_MODBUS, 0},                       // Modbus protocol error
    {ERROR_TYPE_ADC, 0},                          // ADC error
    {ERROR_TYPE_GPIO, 0},                         // GPIO error
    {ERROR_TYPE_CONFIG, 0},                       // Configuration error
    {ERROR_TYPE_USER, 0},                         // User-defined error
    {ERROR_TYPE_TEMPERATURE_SENSOR, 0},           // Temperature sensor error
};

ErrorJournalRecord_t ErrorJournal::errJrnlCountsRecord[1280];

void ErrorJournal::Init(void) {}

bool ErrorJournal::LoadData(void) {
  return false;
  // errJrnl = *(ErrorJournal_t*)0x0;
}
void ErrorJournal::SaveData(void) {
  // FlashService::CheckDiffAndReprogramm(ERROR_JOURNAL_ADDRESS, &errJrnlCounts,
  //                                      sizeof(ErrorJournalCounts_t));
}

void ErrorJournal::CheckWDTRst(void) {
  if ((flag_status)(CRM->ctrlsts_bit.wdtrstf) == SET) {
    // errJrnlCounts.wdt_rst++;
    // SaveData();
  }
}
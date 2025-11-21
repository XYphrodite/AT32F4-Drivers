#include "ErrorJournal.h"
#include "FlashService.h"

ErrorJournalRecordMap_t ErrorJournalRecordMap[] = {
    {ERROR_JOURNAL_RECORD_TYPE_WDT, 0},   
    {ERROR_JOURNAL_RECORD_TYPE_SPI1, 0},
    {ERROR_JOURNAL_RECORD_TYPE_UART1, 0}, 
    {ERROR_JOURNAL_RECORD_TYPE_CAN, 0},
    {ERROR_JOURNAL_RECORD_TYPE_MCU, 0},
};

ErrorJournalRecord_t ErrorJournal::errJrnlCountsRecord[12800];

void ErrorJournal::Init(void) {

}

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
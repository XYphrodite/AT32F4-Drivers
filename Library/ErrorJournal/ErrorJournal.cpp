#include "ErrorJournal.h"
#include "FlashService.h"

ErrorJournalCounts_t ErrorJournal::errJrnlCounts;

void ErrorJournal::Init(void) {

  errJrnlCounts.wdt_rst = 0;
  errJrnlCounts.spi1_rst = 0;
  errJrnlCounts.uart1_rst = 0;
  errJrnlCounts.lora_rst = 0;
  errJrnlCounts.mcu_rst = 0;
}

bool ErrorJournal::LoadData(void) {
    uint32_t size = sizeof(ErrorJournalCounts_t);
    FlashService::Read(ERROR_JOURNAL_ADDRESS, &errJrnlCounts, size);
    uint8_t crc = count_CRC((uint8_t*)&errJrnlCounts, size - 1);
    if (crc != errJrnlCounts.crc) {
        return false;
    }
    return true;
  // errJrnl = *(ErrorJournal_t*)0x0;
}
void ErrorJournal::SaveData(void) {
  FlashService::CheckDiffAndReprogramm(ERROR_JOURNAL_ADDRESS, &errJrnlCounts, sizeof(ErrorJournalCounts_t));
}

void ErrorJournal::CheckWDTRst(void) {
  if ((flag_status)(CRM->ctrlsts_bit.wdtrstf) == SET) {
    errJrnlCounts.wdt_rst++;
    SaveData();
  }
}
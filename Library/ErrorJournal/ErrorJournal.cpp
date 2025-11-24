#include "ErrorJournal.h"
#include "FlashService.h"
#include <cstdint>
#include <cstring>

// ErrorJournalRecord ErrorJournal::errJrnlCountsRecord[1280];

void ErrorJournalRecord::LoadFromFlashByAdress(uint32_t address) {}

void ErrorJournalRecord::LoadFromFlashByIndex(uint16_t index) {}

void ErrorJournal::Init(void) {
  LoadData();
  CheckWDTRst();
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

ErrorJournalRecordMap_t ErrorJournal::GetErrMap(ErrorJournalRecordType type) {
  for (uint16_t i = 0; i < ErrorJournalRecordMapSize; i++) {
    if (ErrorJournalRecordMap[i].type == type) {
      return ErrorJournalRecordMap[i];
    }
  }
  return ErrorJournalRecordMap[0];
}

ErrorJournalRecord::ErrorJournalRecord(const ErrorJournalRecordType type,
                                       const ErrorLevelType level,
                                       const char *message, const bool saving)
    : index(0), address(0), type(type), level(level), timestamp(0) {
  ErrorJournalRecordMap_t map = ErrorJournal::GetErrMap(type);
  uint16_t message_length = strlen(message) <= 26 ? strlen(message) : 26;
  memcpy(_message, message, strlen(message));

  if (saving) {
    ErrorJournal::TryAdd(this);
  }
}

uint16_t ErrorJournal::TryAdd(ErrorJournalRecord* record) {
  return 0;
}
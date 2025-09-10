#include "ErrorJournal.h"

ErrorJournal_t errJrnl;
bool errJrnlhasChange = FALSE;

void ErrorJournal_LoadData(void) {
    return;
    // errJrnl = *(ErrorJournal_t*)0x0;
}
void ErrorJournal_SaveData(void) {
    return;
    uint32_t size = sizeof(ErrorJournal_t);
    uint8_t arr[0xFF];
    memcpy(arr, &errJrnl, size);
    // fs_programm(0x0, arr, size);
}

void ErrorJournal_CheckWDTRst(void) {
    if ((flag_status)(CRM->ctrlsts_bit.wdtrstf) == SET) {
        errJrnl.wdt_rst++;
        // ErrorJournal_SaveData();
    }
}
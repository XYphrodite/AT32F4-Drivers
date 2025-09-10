#ifndef _ERROR_JOURNAL_H_
#define _ERROR_JOURNAL_H_


#include "at32f403a_407.h"
// #include "FlashService.h"
#include "string.h"

    typedef struct ErrorJournal {
        uint32_t wdt_rst;
        uint32_t spi1_rst;
        uint32_t uart1_rst;
        uint32_t lora_rst;
        uint32_t mcu_rst;
    } ErrorJournal_t;

    void ErrorJournal_LoadData(void);
    void ErrorJournal_SaveData(void);
    void ErrorJournal_CheckWDTRst(void);

    extern ErrorJournal_t errJrnl;


#endif

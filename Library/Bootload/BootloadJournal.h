#ifndef _BOOTLOAD_JOURNAL_H_
#define _BOOTLOAD_JOURNAL_H_

#ifndef PREBOOT

#include "at32f403a_407.h"
#include "FlashService.h"
#include "rtc_module.h"

#define SEC_BOOTLOAD_JOURNAL (254U)
#define ADDRESS_BOOTLOAD_JOURNAL_CONGIG (SECTOR_ADDRESS(SEC_BOOTLOAD_JOURNAL))
#define BOOTLOAD_JOURNAL_SIZE (256U) // 2 * 1024 / 8 = 256   

class BootloadJournal
{
private:
static uint16_t size;
static calendar_type journal[BOOTLOAD_JOURNAL_SIZE];
    /* data */
public:
    static void Init(void);
    static void Add(calendar_type time);
    static void EraseJournal(void);
    static uint16_t GetSize(void);
    static calendar_type GetJournalItem(uint16_t num);
    static uint32_t GetAverageWorkTime(void);
    static void CheckWDRTS(void);

    BootloadJournal(/* args */);
    ~BootloadJournal();
};


#endif

#endif

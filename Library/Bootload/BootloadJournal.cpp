#include "BootloadJournal.h"

#ifndef PREBOOT

uint16_t BootloadJournal::size;
calendar_type BootloadJournal::journal[BOOTLOAD_JOURNAL_SIZE];

void BootloadJournal::Init(void)
{
    uint32_t startAddress = ADDRESS_BOOTLOAD_JOURNAL_CONGIG;
    size = 0;
    // Read flash data into journal
    for (size_t i = 0; i < BOOTLOAD_JOURNAL_SIZE; i++)
    {
        journal[i] = *reinterpret_cast<calendar_type *>(startAddress + (i * sizeof(calendar_type)));
        if (*reinterpret_cast<uint32_t *>(&journal[i]) != 0xFFFFFFFF)
        {
            size++;
        }
        else
        {
            break;
        }
    }
}

void BootloadJournal::Add(calendar_type time)
{
    if (size + 1 < BOOTLOAD_JOURNAL_SIZE)
    {
        journal[size] = time;
        FlashService::Programm(ADDRESS_BOOTLOAD_JOURNAL_CONGIG + size * sizeof(calendar_type), reinterpret_cast<uint8_t *>(&time), sizeof(calendar_type));
        size++;
    }
    else
    {
        EraseJournal();
    }
}

void BootloadJournal::EraseJournal(void)
{
    FlashService::EraseSectorByNum(SEC_BOOTLOAD_JOURNAL);
}

uint16_t BootloadJournal::GetSize(void) { return size; }

calendar_type BootloadJournal::GetJournalItem(uint16_t num)
{
    return journal[num];
}

uint32_t BootloadJournal::GetAverageWorkTime(void)
{
    if (size >= 2)
    {
        uint32_t diff =  rtc_time_diff(&journal[size - 1],&journal[0]);
        return diff / (size - 1);
    }
    return 0xF0F0F0F0;
}

void BootloadJournal::CheckWDRTS(void){
    if ((flag_status)(CRM->ctrlsts_bit.wdtrstf) == SET) {
        calendar_type currentTime;
        rtc_module_get_time(&currentTime);
        Add(currentTime);
    }
}

BootloadJournal::BootloadJournal(/* args */)
{
}

BootloadJournal::~BootloadJournal()
{
}

#endif

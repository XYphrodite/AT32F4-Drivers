#ifndef _FW_LOADER_H_
#define _FW_LOADER_H_

#include "at32f403a_407.h"
#include "FlashService.h"
#include "Bootload.h"
#include "CRC.h"
#include "TafcoCrypt.h"

// #define PROGRAMM_BLOCK_SIZE (128U)

#pragma pack(push, 1)
enum UpdateStatus : uint8_t
{
    UnknownUS = 0x0U,
    SuccessUS = 0x1U,
    UnknownErrorUS = 0x2,
    UpdateIsNotSetUS = 0x3,
    WrongBlockNumUS = 0x4,
    WrongAppCrcUS = 0x5,
    TooLargeAppUS = 0x6
};
#pragma pack(pop)

const uint8_t BlockSize = (128U);

class FW_Loader
{
private:
    // static uint32_t programm_address;
    static uint32_t previousBlockNum;
    static bool isUpdating;

public:
    FW_Loader(/* args */);
    ~FW_Loader();
    static void Init(void);
    static void StartUpdate(void);
    static UpdateStatus EraseMainApp(void);
    static void SetBlockAndDecrypt(uint8_t *buff);
    static UpdateStatus Programm(uint32_t blockNum, uint32_t &nextBlock);
    static void StopUpdate(void);
    static UpdateStatus CheckCRC(uint8_t crc, uint32_t length);
    // static bool CheckAppExistence(void);

    static uint8_t FwBlockBuff[BlockSize];
};

#endif
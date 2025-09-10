#include "FW_Loader.h"

// uint32_t FW_Loader::programm_address;
uint32_t FW_Loader::previousBlockNum;
bool FW_Loader::isUpdating;
uint8_t FW_Loader::FwBlockBuff[BlockSize];

FW_Loader::FW_Loader(/* args */)
{
}

FW_Loader::~FW_Loader()
{
}

void FW_Loader::Init(void)
{
    isUpdating = false;
}

void FW_Loader::StartUpdate(void)
{
    FlashService::Init();
    isUpdating = true;
}

UpdateStatus FW_Loader::EraseMainApp(void)
{
    if (!isUpdating)
        return UpdateIsNotSetUS;
    uint16_t max_app_size = BootloadConfig::GetMaxAppSize();
    bool result = FlashService::EraseSectorByAddr(BootloadConfig::GetJumpAddress(), max_app_size / 2); // TODO: make dynamic
    previousBlockNum = 0;
    if (result)
        return SuccessUS;
    else
        return UnknownErrorUS;
}

void FW_Loader::SetBlockAndDecrypt(uint8_t *buff)
{
    memcpy(FW_Loader::FwBlockBuff, buff, KeySize);
    decrypt_firmware_block(FW_Loader::FwBlockBuff);
}

UpdateStatus FW_Loader::Programm(uint32_t blockNum, uint32_t &nextBlock)
{
    if (!isUpdating)
        return UpdateIsNotSetUS;
    if (previousBlockNum + 1 == blockNum)
    {
        uint32_t programm_address = BootloadConfig::GetJumpAddress() + (blockNum - 1) * BlockSize;
        bool result = FlashService::Programm(programm_address, FwBlockBuff, BlockSize);
        if (result)
        {
            previousBlockNum = blockNum;
            nextBlock = blockNum + 1;
            return SuccessUS;
        }
        else
        {
            nextBlock = blockNum;
            return UnknownErrorUS;
        }
    }
    else
    {
        nextBlock = previousBlockNum + 1;
        return WrongBlockNumUS;
    }
}

void FW_Loader::StopUpdate(void)
{
    isUpdating = false;
}

UpdateStatus FW_Loader::CheckCRC(uint8_t crc, uint32_t length)
{
    uint32_t mainAppAddr = BootloadConfig::GetJumpAddress();
    uint8_t app_crc = count_AppCRC(mainAppAddr, length);
    bool isEqual = app_crc == crc;
    if (!isEqual)
    {
        uint16_t num_to_erase = (length / 2048) + ((length % 2048) != 0 ? 1 : 0);
        FlashService::EraseSectorByAddr(mainAppAddr, num_to_erase);
    }
    if (isEqual)
        return SuccessUS;
    else
        return WrongAppCrcUS;
}
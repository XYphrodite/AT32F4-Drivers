#ifndef _LORASPI_H_
#define _LORASPI_H_

#include "at32f403a_407.h"
#include "general_spi.h"

class Lora_SPI : public GENERAL_SPI
{
private:
    static Lora_SPI *instance;
    Lora_SPI(/* args */);
    LoraModule_Cfg* lm_cfg;
    /* data */
public:
    bool NoWaitAfterCmd;
    // Deleting the copy constructor to prevent copies
    Lora_SPI(const Lora_SPI &obj) = delete;
    ~Lora_SPI() {};
    static Lora_SPI *GetInstance();
    void SetWaiting(bool w);
    bool GetWaiting(bool w);
    void Wait(void);
    void SetLoraCfg(LoraModule_Cfg* lm_cfg);
    LoraModule_Cfg* GetLoraCfg(void);
};

#endif
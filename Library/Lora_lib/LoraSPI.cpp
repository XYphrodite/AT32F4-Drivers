#include "LoraSPI.h"

Lora_SPI *Lora_SPI::instance;

Lora_SPI::Lora_SPI(/* args */)
{
}

Lora_SPI *Lora_SPI::GetInstance()
{
    if (instance == nullptr)
        instance = new Lora_SPI();
    return instance;
}

void Lora_SPI::SetLoraCfg(LoraModule_Cfg *lm_cfg)
{
    this->lm_cfg = lm_cfg;
}

LoraModule_Cfg *Lora_SPI::GetLoraCfg(void)
{
    return lm_cfg;
}
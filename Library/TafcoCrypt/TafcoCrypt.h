#ifndef _TAFCO_CRYPT_H_
#define _TAFCO_CRYPT_H_

#include "at32f403a_407.h"
#include <cstring>

extern const uint8_t KeySize;

void decrypt_firmware_block(uint8_t *buff);

#endif
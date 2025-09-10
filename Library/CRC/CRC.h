#ifndef _CRC_H_
#define _CRC_H_

#include <at32f403a_407.h>
#include "Macros.h"
#include <stdint.h>
#include <stddef.h>

// void init_CRC(void);

/// @brief
/// @param buff
/// @param length 255 is max
/// @return
uint8_t count_CRC(const uint8_t *buff8, uint32_t length);

uint8_t count_AppCRC(uint32_t addr, uint32_t length);

#endif

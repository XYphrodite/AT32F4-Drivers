#ifndef _COMMAND_STRUCT_
#define _COMMAND_STRUCT_

#include "at32f403a_407.h"

typedef bool (*CMDHandl_p)(uint8_t *buff);

typedef struct CMDFlags
{
    uint8_t isAuth : 2;
    uint8_t isChangeCFG : 1;
    uint8_t reserved : 5;
} CMDFlags_t;

#define MAP_FLGS(_AUTH, _FMEM) {.isAuth = _AUTH, .isChangeCFG = _FMEM, .reserved = 0x0}

typedef struct
{
    uint8_t preffix;
    uint8_t code;
    uint8_t size;
    CMDHandl_p handl;
    CMDFlags_t flags;
} CMD_Map_t;

#define COMMAND_PREFIX_POS (0U)
#define COMMAND_CODE_POS (1U)
#define COMMAND_PACK_POS (2U)

#endif

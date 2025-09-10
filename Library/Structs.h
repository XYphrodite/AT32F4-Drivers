#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <at32f403a_407.h>
#include "Enums.h"

typedef struct version_e
{
    uint8_t major;
    uint8_t minor;
    uint8_t micro;
    version_type type;
} version_t;

#pragma pack(push, 1)
struct hardware_version_t
{
    device_type t;
    version_t v;
};
#pragma pack(pop)

#endif
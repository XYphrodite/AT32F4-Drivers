#ifndef _ENUMS_H_
#define _ENUMS_H_

#include <at32f403a_407.h>

#pragma pack(push, 1)
enum AppType : uint16_t
{
    unknown_appType = 0x0,
    bootloader0 = 0x1,
    end_device = 0x2,
    lite_gateway = 0x3
};

enum version_type : uint8_t
{
    none = 0x0,
    alpha = 0x10U,
    beta = 0x20U,
    release_candidat = 0x30U,
    release = 0x40U,
    fixed = 0x1,
};

enum device_type : uint16_t
{
    wireless_temperature_sensor = 0x1U,
    lite_gateway_lbs = 0x2U,
    pigstore_pblock = 0x3U
};

#ifdef LITE_GATEWAY

enum LbsFlags : uint32_t
{
    lbs_unknown_board = 0x1U << 0U, // 1
    lbs_old_board = 0x1U << 1U, // 2
    lbs_new_board = 0x1U << 2U  // 4
};

#endif

#pragma pack(pop)

#endif

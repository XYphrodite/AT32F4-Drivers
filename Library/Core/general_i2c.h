#ifndef _GENERAL_I2C_H_
#define _GENERAL_I2C_H_

#include "at32f403a_407.h"
#include "Consts.h"
#ifndef NST118_CHECKER
#include "unitControl.h"
#endif
#include "general_i2c_struct.h"

class GENERAL_I2C
{
private:
    static general_i2c_cfg_t gi2cc;
public:
    GENERAL_I2C(void);
    ~GENERAL_I2C();
    static void SetI2C(general_i2c_cfg_t gi2cc);
    static general_i2c_cfg_t GetI2C(void);
    static error_status hal_write(const uint8_t *buff);
    static error_status hal_read(uint8_t *buff);
    static error_status waitUntilLastByteSend(void);

    static general_i2c_cfg_t GetI2C1(void);
    static void InitI2C(void);
    static void DisableI2C(void);
    static void EnableI2C(void);
};

#endif
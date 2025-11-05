#ifndef _GENERAL_SPI_H_
#define _GENERAL_SPI_H_

#include "at32f403a_407.h"
#include "Consts.h"
#include "unitControl.h"
#include "general_spi_struct.h"


class GENERAL_SPI {
private:

public:
    // void* gsc;
    general_spi_cfg* gsc;
    GENERAL_SPI(/* args */);
    ~GENERAL_SPI();
    error_status hal_write(const uint8_t* buff);
    error_status hal_read(uint8_t* buff);
    error_status waitUntilLastByteSend(void);
    void set_cs(flag_status new_status);
};

#endif
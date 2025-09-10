#ifndef _AUTHORIZE_H_
#define _AUTHORIZE_H_

#ifdef END_DEVICE_TEMPERATURE_SENSOR

#include "at32f403a_407.h"
#include "ModemConfig.h"

class AuthorizeModule
{
private:
    static bool isAuthorized;
    static void SetAuthorized(bool v);

public:
    AuthorizeModule(/* args */);
    ~AuthorizeModule();

    static bool IsAuthorized(void);
    static bool TryAuthorize(const char *pass);
};

#endif

#endif

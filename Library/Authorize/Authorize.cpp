#ifdef END_DEVICE_TEMPERATURE_SENSOR

#include "Authorize.h"

bool AuthorizeModule::isAuthorized;

AuthorizeModule::AuthorizeModule(/* args */)
{
}

AuthorizeModule::~AuthorizeModule()
{
}

void AuthorizeModule::SetAuthorized(bool v)
{
    isAuthorized = v;
}

bool AuthorizeModule::IsAuthorized()
{
    return isAuthorized;
}

bool AuthorizeModule::TryAuthorize(const char *pass)
{
    char stored[4];
    ModemConfig::GetPassword(stored);
    bool isIdentical = memcmp(pass, stored, 4) == 0;
    isAuthorized = isIdentical;
    return isIdentical;
}

#endif

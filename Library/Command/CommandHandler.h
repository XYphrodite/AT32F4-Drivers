#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "at32f403a_407.h"
#include <string.h>
#include "Consts.h"
#include "CommandStruct.h"
#include "CRC.h"
#if (END_DEVICE_TEMPERATURE_SENSOR && !BOOTLOADER)
#include "Authorize.h"
#endif

#pragma pack(push, 1)
enum command_status : uint8_t
{
    UNKNOWN_CS = 0x0,
    SUCCESS_CS = 0x1,
    ERROR_CS = 0x2,
    NO_USER_AUTH_CS = 0x3,
    NO_SERVICE_AUTH_CS = 0x4,
    NO_SUPER_USER_AUTH_CS = 0x5
};
#pragma pack(pop)

class CommandHandler
{
private:
    uint8_t resp[128];
    uint16_t respCnt;

    CMD_Map_t *cmdMap;
    size_t cmd_amount;
    uint8_t lastComCode;
    void (*postCmd)(void);


public:
    CommandHandler(/* args */);
    ~CommandHandler();
    uint8_t GetCommandSize(uint8_t code);
    bool TryGetCommand(uint8_t req[], uint16_t len, uint8_t *outCom, uint16_t *outLen);
    bool Do(uint8_t req[]);

    void SetCommands(CMD_Map_t *cmds, size_t size);
    uint8_t GetLastComCode(void);
    void SetPostCmd(void (*postCmd)(void));
    void DoPostCommand(void);
    void SetResponce(const command_status cs, const void *data, const uint16_t cnt);
    void SetResponce(const command_status cs);

    const uint8_t* GetRespBuff(void);
    uint16_t GetRespCnt(void);

};

extern CommandHandler CmdHandler;

#endif

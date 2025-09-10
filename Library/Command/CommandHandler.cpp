#include "CommandHandler.h"

CommandHandler::CommandHandler(/* args */)
{
}

CommandHandler::~CommandHandler()
{
}

uint8_t CommandHandler::GetCommandSize(uint8_t code)
{
    for (uint16_t i = 0; i < cmd_amount; i++)
    {
        if (cmdMap[i].code == code)
            return cmdMap[i].size;
    }
    return 0;
}

bool CommandHandler::TryGetCommand(uint8_t req[], uint16_t len, uint8_t *outCom, uint16_t *outLen)
{
    for (size_t i = 0; i < len; i++)
    {
        if (req[i] == REQUEST_COM_PREFIX)
        {
            uint8_t code = req[i + COMMAND_CODE_POS];
            uint8_t comSize = GetCommandSize(code);
            if (comSize == 0)
            {
                return false;
            } // command wasn't found
            if (len - i < comSize)
            {
                return false; // command isn't full
            }
            // put to RX
            memcpy(outCom, &req[i], comSize);
            // check CRC
            if (count_CRC(outCom, comSize - 1) != outCom[comSize - 1])
                return false;
            lastComCode = code;
            return true;
        }
    }
    return false;
}

bool CommandHandler::Do(uint8_t req[])
{
    for (uint16_t i = 0; i < cmd_amount; i++)
    {
        if (cmdMap[i].code == req[COMMAND_CODE_POS])
        {
            lastComCode = req[COMMAND_CODE_POS];
// Check auth if required
#if (END_DEVICE_TEMPERATURE_SENSOR && !BOOTLOADER)
            if (cmdMap[i].flags.isAuth && !AuthorizeModule::IsAuthorized())
            {
                SetResponce(NO_USER_AUTH_CS);
                return false;
            }
#endif
            return cmdMap[i].handl(req);
        }
    }
    return false;
}

void CommandHandler::SetCommands(CMD_Map_t *cmds, size_t size)
{
    cmdMap = cmds;
    cmd_amount = size;
}

uint8_t CommandHandler::GetLastComCode(void)
{
    return lastComCode;
}

void CommandHandler::DoPostCommand(void)
{
    if (postCmd != nullptr)
    {
        postCmd();
        postCmd = nullptr;
    }
}

void CommandHandler::SetPostCmd(void (*postCmd)(void))
{
    this->postCmd = postCmd;
}

void CommandHandler::SetResponce(const command_status cs, const void *data, const uint16_t cnt)
{
    uint8_t *cursor = resp;
    respCnt = 0;
    cursor[0] = cs;
    cursor += sizeof(cs);
    respCnt += sizeof(cs);
    if (data != nullptr && cnt != 0)
    {
        memcpy(cursor, data, cnt);
        respCnt += cnt;
    }
}

void CommandHandler::SetResponce(const command_status cs)
{
    uint8_t *cursor = resp;
    respCnt = 0;
    cursor[0] = cs;
    cursor += sizeof(cs);
    respCnt += sizeof(cs);
}

const uint8_t *CommandHandler::GetRespBuff(void)
{
    return resp;
}

uint16_t CommandHandler::GetRespCnt(void)
{
    return respCnt;
}

CommandHandler CmdHandler = CommandHandler();

#include "SdioCmd.h" 

#include "SdioCmd52.h"
#include "SdioCmd53.h"


SdioCmd* SdioCmd::lastSdioHostCmd= 0;
SdioCmd* SdioCmd::CreateSdioCmd(U64 data)
{
    U32 cmd = CMD_VAL(data);
    SdioCmd* ptr = 0;

    switch (cmd)
    {
        case 53:
            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                ptr = new SdioCmd53(data);
                delete lastSdioHostCmd;
                lastSdioHostCmd = ptr;
            }
            else
            {
                ptr = new SdioCmd53Resp(data);
            }
            break;
        case 52:
            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                ptr = new SdioCmd52(data);
                delete lastSdioHostCmd;
                lastSdioHostCmd = ptr;
            }
            else
            {
                ptr = new SdioCmd52Resp(data);
            }
            break;
        default:
            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                delete lastSdioHostCmd;
                lastSdioHostCmd = 0;
            }
            break;
    }
    return  ptr;
}

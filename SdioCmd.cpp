#include "SdioCmd.h" 

#include "SdioCmd5.h"
#include "SdioCmd52.h"
#include "SdioCmd53.h"
#include "CIA.h"

#include "SDIOParser.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

SdioCmd* SdioCmd::lastSdioHostCmd= 0;
const char * SdioCmd::getShortString()
{
    ostringstream stream;
    char format[200] = {0};

    sprintf(format, "0x%012llX", cmdData);

    string str(format);
    const char * chr = str.c_str();

    return chr;
}

const char * SdioCmd::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << "0x" << hex << cmdData  << ", CMD" << getCmd();

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}

SdioCmd* SdioCmd::CreateSdioCmd(U64 data)
{
    U32 cmd = CMD_VAL(data);
    SdioCmd* ptr = 0;

    switch (cmd)
    {
        case 5:
        case 0x3f:

            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                ptr = new SdioCmd5(data);
                delete lastSdioHostCmd;
                lastSdioHostCmd = 0;
            }
            else
            {
                ptr = new SdioCmd5Resp(data);
            }
            break;
        case 52:
            {
                // the cccr will be static, so no need to free it
                CCCR *cccr = CCCR::BuildCCCR(data);

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
                // verification that I cannot delete singleton
                //delete cccr;
            }
            break;
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
        default:
            ptr = new SdioCmd(data);
            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                delete lastSdioHostCmd;
                lastSdioHostCmd = 0;
            }
            break;
    }
    return  ptr;
}

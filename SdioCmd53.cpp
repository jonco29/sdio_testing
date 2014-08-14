#include "SdioCmd53.h"
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

const char * SdioCmd53::getShortString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << "0x" << hex << cmdData << " CMD53 ";
    if (getRead()) stream << "R:";
    else stream << "W:";

    stream << setw(2) << getXferCount() << " ";

    if (isBlockMode()) stream << " Blks";
    else stream << " Bytes";

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}

const char * SdioCmd53::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << hex << cmdData << " CMD53 ";
    stream << "Function: " << getFunctionNumber() << ", ";
    if (getRead()) stream << "Read, ";
    else stream << "Write, ";


    stream << getXferCount();

    if (isBlockMode()) stream << " Blocks, ";
    else stream << " Bytes, ";

    stream << "From Address: " << hex << getRegisterAddress() << ", ";

    if (isIncrementingAddress()) stream << "using Incrementing Addresses";
    else stream << "using Fixed Addressing";

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}
bool SdioCmd53::getRead()
{
    bool retVal = true;
    if (CMD53_RW(cmdData))
    {
        retVal = false;
    }
    return retVal;
}

bool SdioCmd53::getWrite()
{
    return !getRead();
}

U32 SdioCmd53::getFunctionNumber()
{
    U8 fun = CMD53_FUN(cmdData);
}

bool SdioCmd53::isBlockMode()
{
    bool retVal = false;
    if (CMD53_BLOCK_MODE(cmdData))
    {
        retVal = true;
    }
    return retVal;
}
bool SdioCmd53::isByteMode()
{
    return !isBlockMode();
}
bool SdioCmd53::isIncrementingAddress()
{
    bool retVal = false;
    if (CMD53_OP_CODE(cmdData))
    {
        retVal = true;
    }
    return retVal;
}
bool SdioCmd53::isFixedAddress()
{
    return !isIncrementingAddress();
}

U32 SdioCmd53::getRegisterAddress()
{
    U32 address = CMD53_ADDRESS(cmdData);
    return address;
}

U32 SdioCmd53::getXferCount()
{
    U32 count = (U32)CMD53_COUNT(cmdData);
    return count;
}

const char* SdioCmd53Resp::getShortString()
{
    ostringstream stream;
    char format[200] = {0};

    sprintf(format, "0x%012llX CMD53 Resp", cmdData);
    //stream << format;
    // stream << "0x" << hex << cmdData << " CMD53 Resp ";
    // stream << "0x" <<  setw(2) << setfill('0') << hex << getData();

    string str(format);
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}
const char* SdioCmd53Resp::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};
    U32 flags;

    stream << "0x" << hex << cmdData << " CMD53 Rsp ";

    stream << "Data: 0x" <<  setw(2) << setfill('0') << hex << getData() << " ";
    flags = getResponseBitFlags();
    stream << "Flags: 0x"<< hex << setw(2) << setfill('0') << flags << ":";
    if (flags & 0x80) stream << "COM_CRC_ERROR:";
    if (flags & 0x40) stream << "ILLEGAL_COMMAND:";
    switch (flags & 0x30)
    {
        case 0x00:
            stream << "IO_CURRENT_STATE - DIS:";
            break;
        case 0x10:
            stream << "IO_CURRENT_STATE - CMD:";
            break;
        case 0x20:
            stream << "IO_CURRENT_STATE - TRN:";
            break;
        case 0x30:
            stream << "IO_CURRENT_STATE - RFU:";
            break;
        case 0x08:
            stream << "ERROR:";
            break;
        case 0x02:
            stream << "FUNCTION_NUMBER invalid:";
            break;
        case 0x01:
            stream << "OUT_OF_RANGE:";
            break;
    }
    

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}

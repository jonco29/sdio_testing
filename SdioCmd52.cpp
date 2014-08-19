#include "SdioCmd52.h"
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

const char * SdioCmd52::getShortString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << "0x" << hex << cmdData << " CMD52 ";
    if (getRead()) stream << "R:";
    else stream << "W:";

    stream << "0x" <<  setw(2) << setfill('0') << hex << (unsigned int) getData();
    if (getRead()) stream <<" from " << setw(5) << setfill('0') << hex << getRegisterAddress();
    if (getWrite()) stream <<" to " << setw(5) << setfill('0') << hex << getRegisterAddress();

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}

const char * SdioCmd52::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << "0x" << hex << cmdData << " CMD52 ";
    stream << "Function: " << getFunctionNumber() << ", ";
    if (getRead()) stream << "Read, ";
    else stream << "Write, ";


    stream << "0x" <<  setw(2) << setfill('0') << hex << getData();
    if (getRead()) stream <<" from " << setw(5) << setfill('0') << hex << getRegisterAddress();
    if (getWrite()) stream <<" to " << setw(5) << setfill('0') << hex << getRegisterAddress();

    if (isReadAfterWrite()) stream << ", RAW is set";
    else stream << ", RAW is not set";


    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}
bool SdioCmd52::getRead()
{
    bool retVal = true;
    if (CMD52_RW(cmdData))
    {
        retVal = false;
    }
    return retVal;
}

bool SdioCmd52::getWrite()
{
    return !getRead();
}

U32 SdioCmd52::getFunctionNumber()
{
    U8 fun = (U8)CMD52_FUN(cmdData);
}

bool SdioCmd52::isReadAfterWrite()
{
    bool retVal = false;
    if (CMD52_RAW(cmdData))
    {
        retVal = true;
    }
    return retVal;
}


U32 SdioCmd52::getRegisterAddress()
{
    U32 address = CMD52_ADDRESS(cmdData);
    return address;
}

U32 SdioCmd52::getData()
{
    U32 data = (U32)CMD52_DATA(cmdData);
    return data;
}


const char* SdioCmd52Resp::getShortString()
{
    ostringstream stream;
    char format[200] = {0};

    stream << "0x" << hex << cmdData << " CMD52 Resp ";

    stream << "0x" <<  setw(2) << setfill('0') << hex << getData();

    string str = stream.str();
    stream.flush();
    const char * chr = str.c_str();

    return chr;
}
const char* SdioCmd52Resp::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};
    U32 flags;

    stream << "0x" << hex << cmdData << " CMD52 Rsp ";

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

U32 SdioCmd52Resp::getResponseBitFlags()
{
    U32 flags = (U32)CMD52_RESP_FLAGS(cmdData);
    return flags;
}
U32 SdioCmd52Resp::getData()
{
    U32  data = (U32)CMD52_RESP_DATA(cmdData);
    return (U8)data;
}

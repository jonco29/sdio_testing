#include "SdioCmd53.h"
#include "SDIOParser.h"
#include <iostream>
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

    stream << getXferCount();

    if (isBlockMode()) stream << " Blks";
    else stream << " Bytes";

    string str = stream.str();
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
    bool retVal = true;
    if (CMD53_RW(cmdData))
    {
        retVal = false;
    }
    return retVal;
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
    bool retVal = true;
    if (CMD53_BLOCK_MODE(cmdData))
    {
        retVal = false;
    }
    return retVal;
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
    bool retVal = true;
    if (CMD53_OP_CODE(cmdData))
    {
        retVal = false;
    }
    return retVal;
}

U32 SdioCmd53::getRegisterAddress()
{
    U32 address = CMD53_ADDRESS(cmdData);
    return address;
}

U32 SdioCmd53::getXferCount()
{
    U32 count = CMD53_COUNT(cmdData);
    return count;
}

#include "SdioCmd53.h"
#include "SDIOParser.h"

const char * SdioCmd53::getShortString()
{
    return "short string";
}

const char * SdioCmd53::getDetailedString()
{
    return "detailed string";
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

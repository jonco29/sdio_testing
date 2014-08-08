#ifndef SDIOCMD_H
#define SDIOCMD_H

#include "LogicPublicTypes.h"
#include "SDIOParser.h"

class SdioCmd
{
public:
    SdioCmd(U64 data) { cmdData = data;}

    virtual const char* getShortString() = 0;
    virtual const char* getDetailedString() = 0;

    virtual U32 getCmd() { return CMD_VAL(cmdData); };
    virtual U32 getDir() { return CMD_DIR(cmdData); };


protected:
    U64 cmdData;
};


#endif /* SDIOCMD_H */

#ifndef SDIOCMD_H
#define SDIOCMD_H

#include "LogicPublicTypes.h"

class SdioCmd
{
public:
    virtual const char* getShortString() = 0;
    virtual const char* getDetailedString() = 0;
    SdioCmd(U64 data) { cmdData = data;}

protected:
    U64 cmdData;
};


#endif /* SDIOCMD_H */

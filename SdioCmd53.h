#ifndef SDIOCMD53_H
#define SDIOCMD53_H

#include "SdioCmd.h"

class SdioCmd53 : public SdioCmd
{
public:
    SdioCmd53(U64 data):SdioCmd(data){};
    virtual const char* getShortString();
    virtual const char* getDetailedString();

    bool getRead(); 
    bool getWrite(); 
    U32  getFunctionNumber();
    bool isBlockMode();
    bool isByteMode();
    bool isFixedAddress();
    bool isIncrementingAddress();
    U32  getRegisterAddress();
    U32  getXferCount();

};


#endif /* SDIOCMD53_H */

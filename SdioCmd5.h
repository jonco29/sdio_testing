#ifndef SDIOCMD5_H
#define SDIOCMD5_H

#include "SdioCmd.h"

class SdioCmd5 : public SdioCmd
{
public:
    SdioCmd5(U64 data):SdioCmd(data){};
    virtual const char* getDetailedString();

    U32  getOCCR();
};

class SdioCmd5Resp : public SdioCmd
{
public:
    SdioCmd5Resp(U64 data):SdioCmd(data){};
    virtual const char* getShortString();
    virtual const char* getDetailedString();

    U32 getOCCR()           {U32 occr = (U32)CMD5_RESP_OCR(cmdData); return occr;};
    bool isCardReady ()     { bool ready = (bool)CMD5_RESP_C(cmdData); return ready;};
    U32  numIOFunctions()   { U32 num = (U32)CMD5_RESP_NO_FUN(cmdData); return num;};
    bool isMemoryPresent()  { bool mem = (bool)CMD5_RESP_MEM(cmdData); return mem;};
};


#endif /* SDIOCMD5_H */

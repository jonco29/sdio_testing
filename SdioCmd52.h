#ifndef SDIOCMD52_H
#define SDIOCMD52_H

#include "SdioCmd.h"

class SdioCmd52 : public SdioCmd
{
public:
    SdioCmd52(U64 data):SdioCmd(data){};
    virtual const char* getShortString();
    virtual const char* getDetailedString();

    bool getRead(); 
    bool getWrite(); 
    U32  getFunctionNumber();
    bool isReadAfterWrite();
    U32  getRegisterAddress();
    U32  getData();

};

class SdioCmd52Resp : public SdioCmd
{
public:
    SdioCmd52Resp(U64 data):SdioCmd(data){};
    virtual const char* getShortString();
    virtual const char* getDetailedString();

    U32 getResponseBitFlags();
    U32 getData();
};


#endif /* SDIOCMD52_H */

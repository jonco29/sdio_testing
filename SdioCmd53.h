#ifndef SDIOCMD53_H
#define SDIOCMD53_H

#include "SdioCmd.h"

class SdioCmd53 : public SdioCmd
{
public:
    SdioCmd53(U64 data):SdioCmd(data){};
    virtual const char* getShortString();
    virtual const char* getDetailedString();
};


#endif /* SDIOCMD53_H */

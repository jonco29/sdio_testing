#include "CIA.h"
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

CCCR* CCCR::theCCCR = 0;

CCCR* CCCR::BuildCCCR(U64 data)
{
    if (theCCCR == 0)
    {
        theCCCR = new CCCR();
    }
    theCCCR->AddCmd52ToCCCR(data);
    return theCCCR;
}

bool CCCR::AddCmd52ToCCCR(U64 data)
{
    bool retVal = false;
    if (theCCCR != 0)
    {
        // first check if this is cmd 52
        if (CMD_VAL(data) == 52)
        {
            if (CMD_DIR(data) == DIR_FROM_HOST)
            {
                retVal = theCCCR->HandleCmd52Request(data);
            }
            else
            {
                retVal = theCCCR->HandleCmd52Response(data);
            }
        }
    }
    return retVal;

}
bool CCCR::HandleCmd52Request(U64 data)
{
    SdioCmd52 *c52 = new SdioCmd52(data);
    delete c52;
    return true;
}
bool CCCR::HandleCmd52Response(U64 data)
{
    SdioCmd52Resp *c52Resp = new SdioCmd52Resp(data);
    delete c52Resp;
    return true;
}


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
    U32 address = c52->getRegisterAddress();
    
    // first clean up our last host CMD 52
    if (lastHostCmd52 != 0)
    {
        delete lastHostCmd52;
        lastHostCmd52 = 0;
    }
    // make sure we are using funtion 0
    if (c52->getFunctionNumber() == 0)
    {
        // make sure the register address is within CCCR bounds
        if (address >= CCCR_ADDRESS_START && address <= CCCR_ADDRESS_END)
        {
            //cout << "We got CCCR data: " << hex << data << endl;
            lastHostCmd52 = c52;
        }
        else
        {
            cout << "------------------------------------------------- " << address << endl;
            U32 tmp = getCISPointer();
            //if (address >= 0x100 && address <= 0x1ff)
            if (address >= tmp && address <= (tmp + CIS_MAX_SIZE))
            {
                // we're reading CIS
            }
        }
    }

    return true;
}
bool CCCR::HandleCmd52Response(U64 data)
{
    SdioCmd52Resp *c52Resp = new SdioCmd52Resp(data);
    U32 regAddress = 0;
    U32 c_data = 0;
    U8 *cccrData_ptr = (U8*)&cccr_data;
    
    if (lastHostCmd52 != 0)
    {
        regAddress = lastHostCmd52->getRegisterAddress();
        c_data = (U8)c52Resp->getData();
        cccrData_ptr[regAddress] = c_data;
        delete lastHostCmd52;
        lastHostCmd52 = 0;
    }

    delete c52Resp;
    return true;
}

void CCCR::DumpCCCRTable(void)
{
    U32 i;
    U8* tmp = (U8*)&(theCCCR->cccr_data);
    unsigned char buffer[200] = {0};
    for (i = 0; i < 0xff; i++)
    {
        //memset(buffer, 0, 200);
        cout << setw(2) << setfill('0') << hex << "0x" << i << ":   0x" << setw(2) << setfill('0') << hex << tmp[i] << endl;
    }
}

U32 CCCR::getCISPointer()
{
    U32 tmp = 0;
    memcpy (&tmp, &cccr_data.Common_CIS_Pointer, 3);
    return tmp;
}


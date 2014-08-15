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

const char *CCCR_NAMES[] = 
{
    "0x00 CCCR/SDIO Revision           |SDIO|SDIO|SDIO|SDIO | CCCR|CCCR|CCCR|CCCR|    ",
    "0x01 SD Specification Revision    |RFU |RFU |RFU |RFU  |  SD | SD | SD | SD |    ",
    "0x02 I/O Enable                   |IOE7|IOE6|IOE5|IOE4 | IOE3|IOE2|IOE1|RFU |    ",
    "0x03 I/O Ready                    |IOR7|IOR6|IOR5|IOR4 | IOR3|IOR2|IOR1|RFU |    ",
    "0x04 Int Enable                   |IEN7|IEN6|IEN5|IEN4 | IEN3|IEN2|IEN1|IENM|    ",
    "0x05 Int Pending                  |INT7|INT6|INT5|INT4 | INT3|INT2|INT1|RFU |    ",
    "0x06 I/O Abort                    |RFU |RFU |RFU |RFU  | RES |AS2 |AS1 |AS0 |    ",
    "0x07 Bus Interface Control        |CD d|SCSI|ECSI|RFU  | RFU |RFU |BW 1|BW 0|    ",
    "0x08 Card Capability              |4BLS|LSC |E4MI|S4MI | SBS |SRW |SMB |SDC |    ",
    "0x09 Common CIS Ptr [0]           |    |    |    |     |     |    |    |    |    ",
    "0x0A Common CIS Ptr [1]           |    |    |    |     |     |    |    |    |    ",
    "0x0B Common CIS Ptr [2]           |    |    |    |     |     |    |    |    |    ",
    "0x0C Bus Suspend                  |RFU |RFU |RFU |RFU  | RFU |RFU | BR | BS |    ",
    "0x0D Function Select              | DF |RFU |RFU |RFU  | FS3 |FS2 |FS1 |FS1 |    ",
    "0x0E Exec Flags                   |EX7 |EX6 |EX5 |EX4  | EX3 |EX2 |EX1 |EXM |    ",
    "0x0F Ready Flags                  |RF7 |RF6 |RF5 |RF4  | RF3 |RF2 |RF1 |RFM |    ",
    "0x10 FN0 Block Size [0]           |    |    |    |     |     |    |    |    |    ",
    "0x11 FN0 Block Size [1]           |    |    |    |     |     |    |    |    |    ",
    "0x12 Power Control                |RFU |RFU |RFU |RFU  | RFU |RFU |EMPC|SMPC|    ",
    "0x13 High Speed                   |RFU |RFU |RFU |RFU  | RFU |RFU |EHS |SHS |    "
};

const char* FBR_NAMES[] = 
{
    "0x00 CSA Support/FN Intrf Code    |CSAE|CSAS|RFU |RFU  | Func Interface Code|    ",
    "0x01 Extended Intf Code           |    |    |    |     |     |    |    |    |    ",
    "0x02 Power Selection              |RFU |RFU |RFU |RFU  | RFU |RFU |EPS |SPS |    ",
    "0x03 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x04 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x05 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x06 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x07 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x08 RFU                          |RFU |RFU |RFU |RFU  | RFU |RFU |RFU |RFU |    ",
    "0x09 CIS PTR [0]                  |    |    |    |     |     |    |    |    |    ",
    "0x0A CIS PTR [1]                  |    |    |    |     |     |    |    |    |    ",
    "0x0B CIS PTR [2]                  |    |    |    |     |     |    |    |    |    ",
    "0x0C CSA PTR [0]                  |    |    |    |     |     |    |    |    |    ",
    "0x0D CSA PTR [1]                  |    |    |    |     |     |    |    |    |    ",
    "0x0E CSA PTR [2]                  |    |    |    |     |     |    |    |    |    ",
    "0x0F Data Access Window (CSA)     |    |    |    |     |     |    |    |    |    ",
    "0x10 I/O Block Size [0]           |    |    |    |     |     |    |    |    |    ",
    "0x11 I/O Block Size [1]           |    |    |    |     |     |    |    |    |    ",
};

CCCR* CCCR::theCCCR = 0;

CCCR::CCCR() 
    :lastHostCmd52(0) ,
    cccrDataPopulated(false)
{
    memset(&cccr_data, 0, sizeof(CCCR_t));
    memset(&fbr_data, 0, sizeof (FBR_t)*7);
    int i;
    for (i = 0; i < 7; i++)
    {
        fbrDataPopulated[i] = false;
    }
}

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
        else if (address >= 0x100 && address <= 0x7ff)
        {
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
                lastHostCmd52 = c52;
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
    U32 fbrOffset = 0;
    U8 *fbrData_ptr = 0;
    
    if (lastHostCmd52 != 0)
    {
        // handle function 0 stuff
        if (lastHostCmd52->getFunctionNumber() == 0)
        {
            regAddress = lastHostCmd52->getRegisterAddress();
            c_data = (U8)c52Resp->getData();

            if (regAddress >= CCCR_ADDRESS_START && regAddress <= CCCR_ADDRESS_END )
            {
                cccrDataPopulated = true;
                cccrData_ptr[regAddress] = c_data;
            }
            else if (regAddress >= FBR_ADDRESS_START && regAddress <= FBR_ADDRESS_END)
            {
                fbrOffset = (regAddress >> 8);
                fbrData_ptr = (U8*)&fbr_data[(fbrOffset - 1)];
                fbrData_ptr [regAddress & 0xff] = c_data;

                // indicate that the fbr for this function is now populated
                fbrDataPopulated[fbrOffset -1] = true;
            }
        }
        delete lastHostCmd52;
        lastHostCmd52 = 0;
    }

    delete c52Resp;
    return true;
}

// static function -- available to anyone
void CCCR::DumpCCCRTable(void)
{
    U32 i;
    U8* tmp = 0;
    char buffer[400] = {0};

    // since this is static, we cannot do anything unless we have a CCCR
    if (theCCCR != 0)
    {

        tmp = (U8*)&(theCCCR->cccr_data);
        if (theCCCR->cccrDataPopulated)
        {
            cout << endl << "CCCR TABLE  Address range: 0x0000--0x00FF" << endl;
            cout << "=========================================================================================================" << endl;
            for (i = 0; i < NUM_CCCR_ELEMENTS; i++)
            {
                // sprintf(buffer, "ADDR 0x%02X: 0x%02X -- " PRINTF_BIT_PATTERN,
                //         i, tmp[i], PRINTF_BIT(tmp[i]));
                sprintf(buffer, "%s 0x%02X -- " PRINTF_BIT_PATTERN, CCCR_NAMES[i], tmp[i], PRINTF_BIT(tmp[i]));

                cout << buffer << endl;
            }
            cout << "=========================================================================================================" << endl;
        }
    }
}

// static function
void CCCR::DumpFBRTable(void)
{
    U32 i,funcNo, funcIndex;
    U8* tmp = (U8*)&(theCCCR->fbr_data);
    char buffer[400] = {0};

    if (theCCCR != 0)
    {
        for (funcIndex = 0; funcIndex < 7; funcIndex++)
        {
            if (theCCCR->fbrDataPopulated[funcIndex])
            {
                funcNo = funcIndex + 1;
                tmp = (U8*)&(theCCCR->fbr_data[funcIndex]);

                cout << endl << "FBR TABLE for Function " << funcNo << "  Address range: 0x0"<< funcNo <<"00--0x0" << funcNo <<"FF" << endl;
                cout << "=========================================================================================================" << endl;
                for (i = 0; i < NUM_FBR_ELEMENTS; i++)
                {
                    // sprintf(buffer, "ADDR 0x%02X: 0x%02X -- " PRINTF_BIT_PATTERN,
                    //         i, tmp[i], PRINTF_BIT(tmp[i]));
                    sprintf(buffer, "%s 0x%02X -- " PRINTF_BIT_PATTERN, FBR_NAMES[i], tmp[i], PRINTF_BIT(tmp[i]));

                    cout << buffer << endl;
                }
                cout << "=========================================================================================================" << endl;
            }
        }
    }
}

U32 CCCR::getCISPointer()
{
    U32 tmp = 0;
    memcpy (&tmp, &cccr_data.Common_CIS_Pointer, 3);
    return tmp;
}


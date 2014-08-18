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
    int i;
    memset(&cccr_data, 0, sizeof(CCCR_t));

    for (i = 0; i < 7; i++)
    {
        fbr[i] = FBR(i+1);
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
            lastHostCmd52 = c52;
            // //if (address >= 0x100 && address <= 0x1ff)
            // if (address >= tmp && address <= (tmp + CIS_MAX_SIZE))
            // {
            //     // we're reading CIS
            //     lastHostCmd52 = c52;
            // }
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
    int i;
    
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


        }
        // try to add to all FBR's, they will check addressing, and recursively add the 
        // CIS and CSA pointer addresses
        for (i = 0; i < 7; i++)
        {
            fbr[i].addData(data);
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
    char buffer[400] = {0};

    if (theCCCR != 0)
    {
        for (funcIndex = 0; funcIndex < 7; funcIndex++)
        {
            theCCCR->fbr[funcIndex].DumpFBR();
        }
    }
}

U32 CCCR::getCISPointer()
{
    U32 tmp = 0;
    memcpy (&tmp, &cccr_data.Common_CIS_Pointer, 3);
    return tmp;
}

CCCR::FBR::FBR(U32 number)
: functionNumber(number),
    fbrDataPopulated(false),
    lastTupleAddress(0),
    newTuplePending(false)
{
    memset(&fbr_data, 0, sizeof (FBR_t));
    fbr_data.CIS_ptr[0] = FBR_PTR_INIT_VAL >> 16;
    fbr_data.CIS_ptr[1] = (FBR_PTR_INIT_VAL >> 8) & 0xff;
    fbr_data.CIS_ptr[2] = (FBR_PTR_INIT_VAL ) & 0xff;

    fbr_data.CSA_ptr[0] = FBR_PTR_INIT_VAL >> 16;
    fbr_data.CSA_ptr[1] = (FBR_PTR_INIT_VAL >> 8) & 0xff;
    fbr_data.CSA_ptr[2] = (FBR_PTR_INIT_VAL ) & 0xff;
}

void CCCR::FBR::DumpFBR()
{
    U32 i,funcNo;
    U8* tmp = (U8*)&fbr_data;
    char buffer[400] = {0};

    if (fbrDataPopulated)
    {
        funcNo = 1;
        tmp = (U8*)&fbr_data;

        cout << endl << "FBR TABLE for Function " << funcNo << "  Address range: 0x0"<< funcNo <<"00--0x0" << funcNo <<"FF" << endl;
        cout << "=========================================================================================================" << endl;
        for (i = 0; i < NUM_FBR_ELEMENTS; i++)
        {
            sprintf(buffer, "%s 0x%02X -- " PRINTF_BIT_PATTERN, FBR_NAMES[i], tmp[i], PRINTF_BIT(tmp[i]));

            cout << buffer << endl;
        }
        cout << "=========================================================================================================" << endl;
        cout << "CIS Address is 0x:" << getCisAddress() << endl;
    }
}

void CCCR::FBR::addData(U64 data)
{
    SdioCmd52 *c52 = theCCCR->lastHostCmd52;
    SdioCmd52Resp *c52Resp = new SdioCmd52Resp(data);
    U32 regAddress = 0;
    U32 c_data = 0;
    U8 *fbrData_ptr = 0;
    U32 reqFunctionNumber = 0;
    U32 cisAddress = 0;

    regAddress = c52->getRegisterAddress();
    c_data = (U8)c52Resp->getData();

    // first see if this is a part of the FBR itself, i.e. addressed within
    // the range of this FBR
    if (regAddress >= FBR_ADDRESS_START && regAddress <= FBR_ADDRESS_END)
    {
        reqFunctionNumber = (regAddress >> 8);
        if (reqFunctionNumber == functionNumber)
        {
            fbrData_ptr = (U8*)&fbr_data;
            fbrData_ptr [regAddress & 0xff] = c_data;

            // indicate that the fbr for this function is now populated
            fbrDataPopulated = true;
        }
    }

    // Now, we need to check if this falls into the CIS portion of the FBR.
    // The FBR contains a ptr to the CIS, and this is our jumping off point.
    // we maintain the initial and the last address to use for our range checkers

    cisAddress = getCisAddress();
    // make sure CIS Address has been set to a value, and not the initialization value
    if (cisAddress != 0xdeadbf)
    {
        if (regAddress == cisAddress)
        {
            // we are building the CIS now, this is the first step.  we need to extract data and setup our
            // end addresses for the range checking, etc.
            cout <<"++++++++++++++++++++++++++++++++++++++++++++++ building cis: 0x" << hex << getCisAddress() <<endl;
            newTuplePending = true;
        }
        else if (regAddress == lastTupleAddress)
        {
            if (c_data == 0xff)
            {
                // done with tuple walk
                cout <<"++++++++++++++++++++++++++++++++++++++++++++++ tuple is done: 0x" << hex << getCisAddress() << ", data: 0x" << hex << c_data << endl;
            }
            else
            {
                cout <<"++++++++++++++++++++++++++++++++++++++++++++++ time for next tuple: 0x" << hex << lastTupleAddress << ", data: 0x" << hex << c_data << endl;
                newTuplePending = true;
            }
        }
        else if ((regAddress < lastTupleAddress) && (regAddress > cisAddress))
        {
            cout <<"++++++++++++++++++++++++++++++++++++++++++++++ generic tuple read: 0x" << hex << regAddress << ", data: 0x" << hex << c_data << endl;
        }
        else
        {
            if (newTuplePending)
            {
                newTuplePending = false;
                lastTupleAddress = regAddress + c_data + 1;
                cout <<"++++++++++++++++++++++++++++++++++++++++++++++ next tuple is at: 0x" << hex << lastTupleAddress<< endl;

            }
        }
    }

}
U32 CCCR::FBR::getCisAddress(void)
{
    U32 address = 0;
    address = (fbr_data.CIS_ptr[0] << 16) | (fbr_data.CIS_ptr[1] << 8) | (fbr_data.CIS_ptr[2] << 0);
    return address;
}
void CCCR::FBR::dumpCIS(void)
{
}

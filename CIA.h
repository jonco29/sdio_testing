#ifndef CIA_H
#define CIA_H

#include "LogicPublicTypes.h"
#include "SDIOParser.h"

#define PACKED __attribute__ ((__packed__))

typedef struct PACKED 
{
    unsigned char CCCR_SDIO_Revision;
    unsigned char SD_Specification_Revision;
    unsigned char IO_Enable;
    unsigned char IO_Ready;
    unsigned char Int_Enable;
    unsigned char Int_Pending;
    unsigned char IO_Abort;
    unsigned char Bus_Interface_Control;
    unsigned char Card_Capatility;
    unsigned char Common_CIS_Pointer[3];
    unsigned char Bus_Suspend;
    unsigned char Function_Select;
    unsigned char Exec_Flags;
    unsigned char Ready_Flags;
    unsigned char FN0_Block_Size[2];
    unsigned char Power_Control;
    unsigned char High_Speed;
    unsigned char RFU[0xDC];
    unsigned char Reserved_for_Vendors[0x0F];
} CCCR_t;

// class SdioCmd
// {
// public:
//     SdioCmd(U64 data) { cmdData = data;}
// 
//     virtual const char* getShortString() = 0;
//     virtual const char* getDetailedString() = 0;
// 
//     virtual U32 getCmd() { return CMD_VAL(cmdData); };
//     virtual U32 getDir() { return CMD_DIR(cmdData); };
// 
//     static SdioCmd* CreateSdioCmd(U64 data);
// 
// 
// 
// protected:
//     U64 cmdData;
//     static SdioCmd* lastSdioHostCmd;
// };
// 
// 
#endif /* CIA_H */

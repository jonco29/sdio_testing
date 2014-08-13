#ifndef CIA_H
#define CIA_H

#include "LogicPublicTypes.h"
#include "SDIOParser.h"
#include "SdioCmd52.h"
#include <string.h>

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
// public:
//     CCCR_t() : 
//         CCCR_SDIO_Revision(0),
//         SD_Specification_Revision(0),
//         IO_Enable(0),
//         IO_Ready(0),
//         Int_Enable(0),
//         Int_Pending(0),
//         IO_Abort(0),
//         Bus_Interface_Control(0),
//         Card_Capatility(0),
//         Common_CIS_Pointer[3](0),
//         Bus_Suspend(0),
//         Function_Select(0),
//         Exec_Flags(0),
//         Ready_Flags(0),
//         FN0_Block_Size[2](0),
//         Power_Control(0),
//         High_Speed(0),
//         RFU[0xDC](0),
//         Reserved_for_Vendors[0x0F](0){};
} CCCR_t;

typedef struct PACKED
{
    unsigned char FN_CSA;
    unsigned char FN_extended_SDIO_fn_interface_code;
    unsigned char RFU[6];
    unsigned char CIS_ptr[3];
    unsigned char CSA_ptr[3];
    unsigned char Data_Access_Window_To_CSA;
    unsigned char IO_block_size_for_fn[2];
    unsigned char RFU2[0xEE];
} FBR_t;;

class CCCR
{
    public:
        static CCCR* BuildCCCR(U64 data);
        static bool AddCmd52ToCCCR(U64 data);

    private:
        // variables
        CCCR_t cccr_data;
        static CCCR* theCCCR;
        SdioCmd52 *lastHostCmd52;

        // functions
        CCCR() :lastHostCmd52(0) {memset(&cccr_data, 0, sizeof(cccr_data));};
        bool HandleCmd52Request(U64 data);
        bool HandleCmd52Response(U64 data);

};

// CIS -- defined in 16.7 of simplified SDIO spec

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

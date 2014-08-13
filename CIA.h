#ifndef CIA_H
#define CIA_H

#include "LogicPublicTypes.h"
#include "SDIOParser.h"
#include "SdioCmd52.h"
#include <string.h>

#define PACKED __attribute__ ((__packed__))

#define CCCR_ADDRESS_START 0x00
#define CCCR_ADDRESS_END   0xFF

// this is arbitrary -- just looking at the map in 6.7 of the simplified spec
#define CIS_MAX_SIZE 0xFFF

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
        static void DumpCCCRTable(void);
        U32 getCISPointer();

    private:
        // variables
        CCCR_t cccr_data;
        static CCCR* theCCCR;
        SdioCmd52 *lastHostCmd52;
        

        // functions
        CCCR() :lastHostCmd52(0) {memset(&cccr_data, 0, sizeof(cccr_data));};
        ~CCCR() {};
        CCCR(const CCCR& c) {};
        CCCR& operator=(const CCCR&)  {};

        bool HandleCmd52Request(U64 data);
        bool HandleCmd52Response(U64 data);

};

#endif /* CIA_H */

#ifndef CIA_H
#define CIA_H

#include "LogicPublicTypes.h"
#include "SDIOParser.h"
#include "SdioCmd52.h"
#include <string.h>

#define PACKED __attribute__ ((__packed__))

#define PRINTF_BIT_PATTERN "| %x%x%x%x | %x%x%x%x |"
#define PRINTF_BIT(x) (1 & x >> 7), \
                      (1 & x >> 6), \
                      (1 & x >> 5), \
                      (1 & x >> 4), \
                      (1 & x >> 3), \
                      (1 & x >> 2), \
                      (1 & x >> 1), \
                      (1 & x )

#define CCCR_ADDRESS_START 0x00
#define CCCR_ADDRESS_END   0xFF
#define NUM_CCCR_ELEMENTS 0x14

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

extern const char *CCCR_NAMES[];


// the FBR is an array of 256 bytes for functions 1-7, each function addressed at the next 100 (hex)
// comments for fn1, but they apply to all the fbr's
typedef struct PACKED
{
    unsigned char FN_CSA;                                       // 0x100
    unsigned char FN_extended_SDIO_fn_interface_code;           // 0x101
    unsigned char RFU[7];                                       // 0x102--0x108
    unsigned char CIS_ptr[3];                                   // 0x109-0x10b
    unsigned char CSA_ptr[3];                                   // 0x10c--10e
    unsigned char Data_Access_Window_To_CSA;                    // 0x10f
    unsigned char IO_block_size_for_fn[2];                      // 0x110--0x111
    unsigned char RFU2[0xEE];                                   // 0x112--0x1ff
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
        FBR_t fbr_data[7];
        static CCCR* theCCCR;
        SdioCmd52 *lastHostCmd52;
        

        // functions
        //CCCR() :lastHostCmd52(0) {memset(&cccr_data, 0, sizeof(cccr_data));};
        CCCR();
        ~CCCR() {};
        CCCR(const CCCR& c) {};
        CCCR& operator=(const CCCR&)  {};

        bool HandleCmd52Request(U64 data);
        bool HandleCmd52Response(U64 data);

};

#endif /* CIA_H */

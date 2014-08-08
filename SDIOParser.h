#ifndef SDIOParser_H
#define SDIOParser_H

#define DIR_FROM_HOST   1
#define DIR_FROM_CARD   0

// generic parsing
#define CMD_VAL(x)          ((x >> 40) & 0x3f)
#define CMD_DIR(x)          ((x >> 46) & 0x01)

// specific parsing
// CMD52 Command Fields
#define CMD52_RW(x)         ((x >> 39) & 0x01)
#define CMD52_FUN(x)        ((x >> 36) & 0x07)
#define CMD52_RAW(x)        ((x >> 35) & 0x01)
#define CMD52_ADDRESS(x)    ((x >> 17) & 0x3ffff)
#define CMD52_DATA(x)       ((x >> 8) & 0xff)

// CMD52 Response fields
#define CMD52_RESP_FLAGS(x) ((x >> 16) & 0xff)
#define CMD52_RESP_FLAGS(x) ((x >> 16) & 0xff)
#define CMD52_RESP_DATA(x)  ((x >> 8) & 0xff)

// CMD53 Command Fields
#define CMD53_RW(x)         ((x >> 39) & 0x01)
#define CMD53_FUN(x)        ((x >> 36) & 0x07)
#define CMD53_BLOCK_MODE(x) ((x >> 35) & 0x01)
#define CMD53_OP_CODE(x)    ((x >> 34) & 0x01)
#define CMD53_ADDRESS(x)    ((x >> 17) & 0x3ffff)
#define CMD53_COUNT(x)      ((x >> 8) & 0x1ff)

// CMD5 command and Response Fields
#define CMD5_OCR(x)         ((x >> 8) & 0xffffff)
#define CMD5_RESP_C(x)      ((x >> 39) & 0x1)
#define CMD5_RESP_NO_FUN(x) ((x >> 36) & 0x7)
#define CMD5_RESP_MEM(x)    ((x >> 32) & 0x1)
#define CMD5_RESP_OCR(x)    ((x >> 8) & 0xffffff)

//CMD3 command and response fields
// CMD3 command is basically the command field.  the entire argument is
// stuff bits which shall be ignored per the sd spec
#define CMD3_RESP_RCA(x)    ((x >> 24) & 0xffff)
#define CMD3_RESP_STATUS(x) ((x >> 8) & 0xffff)

extern const char* parse_str(unsigned long val);
extern const char* parse_CMD52(unsigned long val);
extern const char* parse_CMD52_COMMAND(unsigned long val);
extern const char* parse_CMD52_RESP(unsigned long val);

extern const char* parse_CMD53(unsigned long val);
extern const char* parse_CMD53_COMMAND(unsigned long val);
extern const char* parse_CMD53_RESP(unsigned long val);

extern const char* parse_CMD5(unsigned long val);
extern const char* parse_CMD5_COMMAND(unsigned long val);
extern const char* parse_CMD5_RESP(unsigned long val);
extern const char* parse_CMD5_OCR(unsigned int val);

extern const char* parse_CMD3(unsigned long val);
extern const char* parse_CMD3_COMMAND(unsigned long val);
extern const char* parse_CMD3_RESP(unsigned long val);


#endif // SDIOParser_H

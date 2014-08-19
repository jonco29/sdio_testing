#include "SdioCmd5.h"
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


const char * SdioCmd5::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};
    U32 occr = getOCCR();

    stream << "0x" << hex << cmdData << " CMD5 ";
    stream << "OCCR: " << hex << setw(6) << setfill('0') << occr << " ";

    stream << parse_CMD5_OCR(occr);
    stream << " ";

    string str = stream.str();
    const char * chr = str.c_str();

    return chr;
}

U32 SdioCmd5::getOCCR()
{
    U32 occr = (U32)CMD5_OCR(cmdData);
    return occr;
}

const char* SdioCmd5Resp::getShortString()
{
    ostringstream stream;
    char format[200] = {0};
    U32 occr = getOCCR();

    stream << "0x" << hex << cmdData << " CMD5 Resp ";

    stream << "OCR: "<< "0x" <<  setw(6) << setfill('0') << hex << occr;

    string str = stream.str();
    const char * chr = str.c_str();

    return chr;
}
const char* SdioCmd5Resp::getDetailedString()
{
    ostringstream stream;
    char format[200] = {0};
    U32 flags;

    stream << "0x" << hex << cmdData << " CMD5 Rsp ";
    stream << "Card Ready: " << isCardReady() << ", ";
    stream << "Num Functions: " << numIOFunctions() << ", ";
    stream << "Memory Present: " << isMemoryPresent() << ", ";

    stream << "OCR: "<< "0x" <<  setw(6) << setfill('0') << hex << (unsigned int)getOCCR();

    

    string str = stream.str();
    const char * chr = str.c_str();

    return chr;
}


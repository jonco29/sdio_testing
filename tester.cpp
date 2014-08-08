#include <iostream> 
#include "SdioCmd53.h"
using namespace std;


int main (int argc, char** argv)
{
    U64 data = 0x7514000068F9;
    SdioCmd *cmd = new SdioCmd53(data);

    cout << "short string: " << cmd->getShortString() << endl;
    cout << "short string: " << cmd->getDetailedString() << endl;
    cout << "CMD: " << hex << cmd->getCmd() << endl;;
    cout << "DIR: " << hex << cmd->getDir() << endl;;

    cout << "Read: " << ((SdioCmd53*)cmd)->getRead() << endl;
    cout << "Write: " << ((SdioCmd53*)cmd)->getWrite() << endl;

    cout << "Function Number: " << ((SdioCmd53*)cmd)->getFunctionNumber() << endl;
    // bool isBlockMode();
    // bool isByteMode();
    // bool isFixedAddress();
    // bool isIncrementingAddress();
    cout << "BlockMode: " << ((SdioCmd53*)cmd)->isBlockMode() << endl;
    cout << "ByteMode: " << ((SdioCmd53*)cmd)->isByteMode() << endl;
    cout << "Fixed Address: " << ((SdioCmd53*)cmd)->isFixedAddress() << endl;
    cout << "Incrementing Address: " << ((SdioCmd53*)cmd)->isIncrementingAddress() << endl;
    cout << "Register Address: 0x" << hex << ((SdioCmd53*)cmd)->getRegisterAddress() << endl;
    cout << "Transfer Count (byte/block): " << ((SdioCmd53*)cmd)->getXferCount() << endl;

}

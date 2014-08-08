#include "SdioCmd53.h"
#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


void canned();
void process(U64 data);
void processCmd53(U64 data);
int main (int argc, char** argv)
{
    U64 input;
    if(argc <= 1)
    {
        canned();
    }
    else
    {
       ifstream inStream(argv[1], fstream::in);

        if (inStream.is_open())
        {
            while (!inStream.eof())
            {
                inStream >> uppercase >> hex >> input;
                //cout << "jonathan " << uppercase << hex << input << " " << input - 1 <<endl;
                cout << hex << input << "  -- " << endl;
                //cout << parse_str(input) << endl;
                process(input);
            }

            inStream.close();
        }
    }
    return 0;
}

void canned()
{
    U64 data = 0x7514000068F9;
    SdioCmd *cmd = new SdioCmd53(data);

    cout << "short string: " << cmd->getShortString() << endl;
    cout << "detailed string: " << cmd->getDetailedString() << endl;
    cout << "CMD: " << hex << cmd->getCmd() << endl;;
    cout << "DIR: " << hex << cmd->getDir() << endl;;

    cout << "Read: " << ((SdioCmd53*)cmd)->getRead() << endl;
    cout << "Write: " << ((SdioCmd53*)cmd)->getWrite() << endl;

    cout << "Function Number: " << ((SdioCmd53*)cmd)->getFunctionNumber() << endl;
    cout << "BlockMode: " << ((SdioCmd53*)cmd)->isBlockMode() << endl;
    cout << "ByteMode: " << ((SdioCmd53*)cmd)->isByteMode() << endl;
    cout << "Fixed Address: " << ((SdioCmd53*)cmd)->isFixedAddress() << endl;
    cout << "Incrementing Address: " << ((SdioCmd53*)cmd)->isIncrementingAddress() << endl;
    cout << "Register Address: 0x" << hex << ((SdioCmd53*)cmd)->getRegisterAddress() << endl;
    cout << "Transfer Count (byte/block): " << ((SdioCmd53*)cmd)->getXferCount() << endl;

}
void process(U64 data)
{
    U32 cmd = CMD_VAL(data);

    switch (cmd)
    {
        case 53:
            processCmd53(data);
            break;
        default:
            break;
    }
}
void processCmd53(U64 data)
{
    SdioCmd53 *cmd = new SdioCmd53(data);

    cout << "short string: " << cmd->getShortString() << endl;
    cout << "detailed string: " << cmd->getDetailedString() << endl;
    // cout << "CMD: " << hex << cmd->getCmd() << endl;;
    // cout << "DIR: " << hex << cmd->getDir() << endl;;

    // cout << "Read: " << cmd->getRead() << endl;
    // cout << "Write: " << cmd->getWrite() << endl;

    // cout << "Function Number: " << cmd->getFunctionNumber() << endl;
    // cout << "BlockMode: " << cmd->isBlockMode() << endl;
    // cout << "ByteMode: " << cmd->isByteMode() << endl;
    // cout << "Fixed Address: " << cmd->isFixedAddress() << endl;
    // cout << "Incrementing Address: " << cmd->isIncrementingAddress() << endl;
    // cout << "Register Address: 0x" << hex << cmd->getRegisterAddress() << endl;
    // cout << "Transfer Count (byte/block): " << cmd->getXferCount() << endl;

}

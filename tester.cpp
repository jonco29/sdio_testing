#include <iostream> 
#include "SdioCmd53.h"
using namespace std;


int main (int argc, char** argv)
{
    U64 data = 0x7514000068F9;
    SdioCmd *cmd = new SdioCmd53(data);
    SdioCmd53 cmd53(data);

    cout << "hello world" << endl;
    cout << "short string: " << cmd53.getShortString() << endl;
    cout << "short string: " << cmd53.getDetailedString() << endl;

    cout << "short string: " << cmd->getShortString() << endl;
    cout << "short string: " << cmd->getDetailedString() << endl;
}

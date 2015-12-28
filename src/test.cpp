#include "GPGPasswdManager.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <array>
using std::ifstream;
int main()
{
    LOG::restartLog();
    GPGPasswdManager manager;

    //manager.readEncrypted("./test.txt");
    //
    //==Loading cipher from buffer====
    std::string fileName{"./test.txt"};
    ifstream is(fileName, ifstream::binary);
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    std::array<char, 1024> buffer{'\0'};
    is.read(&(buffer[0]), length);


    //manager.listKeys();
    //manager.selectKey(1);
    //manager.encrypt(std::string{&(buffer[0])});
    manager.loadEncryptedFromMem(&(buffer[0]), length);
    std::cout<<manager.dumpRecord()<<std::endl;
    return 0;
}

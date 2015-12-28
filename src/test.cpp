#include "GPGPasswdManager.hpp"
#include "log.hpp"
#include <iostream>
int main()
{
    LOG::restartLog();
    GPGPasswdManager manager;
    manager.readEncrypted("./test.txt");
    std::cout<<manager.dumpRecord()<<std::endl;
    return 0;
}

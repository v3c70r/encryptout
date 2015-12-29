#include "App.hpp"
#include <sstream>


/**
 * @brief Add a string to cipher
 *
 * @param record the string to add
 */
void App::addRecord(const std::string& record, const std::string& fileName)
{
    std::stringstream ss;
    std::cout<<"Dumping record\n";
    ss<<manager.dumpRecord()<<record<<std::endl;
    manager.encrypt(ss.str());
    ss.str(std::string());
    manager.writeCipher(fileName);
    
}

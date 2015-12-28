
/**
 * @brief Interface for passwd manager
 */
#pragma once
#include <string>
#include <vector>
class PasswdManager
{
public:
    virtual void encrypt() = 0;
    virtual void addRecord(const std::string& record)=0;
    virtual std::string dumpRecord() = 0;
    virtual void readEncrypted(std::string fileName)=0;
    virtual ~PasswdManager(){}
protected:
    std::vector<char> encryptedText;
    std::string encryptedFileName;
};

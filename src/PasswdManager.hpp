
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
    virtual std::string findRecord(std::string keyword) = 0;
    virtual void readEncrypted(std::string fileName)=0;
    virtual ~PasswdManager();
protected:
    std::vector<char> encryptedText;
    std::string encryptedFileName;
};

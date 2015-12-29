
/**
 * @brief Interface for passwd manager
 */
#pragma once
#include <string>
#include <vector>
class PasswdManager
{
public:
    virtual void encrypt(const std::string &) = 0;
    virtual std::string dumpRecord() const= 0;
    virtual ~PasswdManager(){}
protected:
};

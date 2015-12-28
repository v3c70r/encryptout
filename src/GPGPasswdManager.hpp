#pragma once
#include "PasswdManager.hpp"
#include "log.hpp"
#include <gpgme.h>
#include <stdexcept>
#include <sstream>

class GPGPasswdManager: public PasswdManager
{
public:
    GPGPasswdManager();

    void encrypt() override;
    std::string dumpRecord() override;

    /*Load file*/
    void readEncrypted(std::string fileName) override;
    ~GPGPasswdManager() override;
    void selectKey(size_t keyIdx);
    void addRecord(const std::string &) override;
private:
    /*GPG Context*/
    gpgme_ctx_t context;
    std::vector<gpgme_key_t> GPGkeys;
    std::vector<std::string> records;

    /*GPG Error check function*/
    void checkGPGError(const gpgme_error_t& err)
    {
        if (err){
            std::stringstream ss;
            ss<<"GPG Error: "<<err<<std::endl;
            LOG::writeLogErr((ss.str()).c_str());
            throw std::runtime_error("GPG Error");
        }
    }
};

#pragma once
#include "PasswdManager.hpp"
#include "log.hpp"
#include <gpgme.h>
#include <stdexcept>
#include <sstream>
#include <array>
#include <iostream>

class GPGPasswdManager: public PasswdManager
{
public:
    GPGPasswdManager();

    /**
     * @brief Encrypt given string
     *
     * @param The string to be encrypted
     */
    void encrypt(const std::string &) override;

    /**
     * @brief Dump plain text
     *
     * @return Dumpped plain text
     */
    std::string dumpRecord() const override;

    /**
     * @brief Load cipher from file
     *
     * @param fileName file that contains the cipher
     */
    void readEncrypted(std::string fileName);

    /**
     * @brief Load cipher from buffer
     *
     * @param buffer cipher
     * @param sz size of cipher in byte
     */
    void loadEncryptedFromMem(const char* buffer, const size_t& sz);
    /**
     * @brief Release GPGME context and keys
     */
    ~GPGPasswdManager() override;
    /**
     * @brief Select a key for encryption
     *
     * @param keyIdx 0 based key index listed by *listkey()*
     */
    void selectKey(size_t keyIdx);
    void listKeys() const
    {
        std::cout<<"Listing private keys...\n";
        for (const auto& key: GPGkeys)
        {
            std::cout<<key->subkeys->keyid<<'\t';
            if (key->uids && key->uids->name)
                std::cout<<key->uids->name<<'\t';
            if (key->uids && key->uids->email)
                std::cout<<key->uids->email;
            std::cout<<std::endl;
        }
    }
private:
    /*GPG Context*/
    gpgme_ctx_t context;
    std::vector<gpgme_key_t> GPGkeys;

    std::vector<char> encryptedText;
    std::array<gpgme_key_t, 2> recp;

    /*GPG Error check function*/
    void checkGPGError(const gpgme_error_t& err) const
    {
        if (err){
            std::stringstream ss;
            ss<<"GPG Error: "<<err<<std::endl;
            LOG::writeLogErr((ss.str()).c_str());
            throw std::runtime_error("GPG Error");
        }
    }
};

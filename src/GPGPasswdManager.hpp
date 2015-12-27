#pragma once
#include "PasswdManager.hpp"
#include <gpgme.h>

class GPGPasswdManager: public PasswdManager
{
public:
    GPGPasswdManager();
    void encrypt() override;
    std::string findRecord(std::string keyword) override;
    void readEncrypted(std::string fileName) override;
private:
    /*GPG Context*/
    gpgme_error_t error;
    gpgme_engine_info_t info;
    gpgme_ctx_t context;
    gpgme_key_t recipients[2] = {NULL, NULL};
    gpgme_data_t clear_text, encrypted_text;
    gpgme_encrypt_result_t  result;
    gpgme_user_id_t user;
};

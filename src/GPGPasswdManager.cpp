#include <fstream>
#include <iostream>
#include "GPGPasswdManager.hpp"
#include <cstring>
using std::ifstream;
using std::runtime_error;


/**
 * @brief Constructer, initialize GPGME
 */
GPGPasswdManager::GPGPasswdManager()
{
    //gpgme_sig_mode_t sigMode = GPGME_SIG_MODE_CLEAR;
    recp.fill(nullptr);
    /* Begin setup of GPGME */
    gpgme_check_version (NULL);
    setlocale (LC_ALL, "");
    gpgme_set_locale (NULL, LC_CTYPE, setlocale (LC_CTYPE, NULL));
#ifndef HAVE_W32_SYSTEM
    gpgme_set_locale (NULL, LC_MESSAGES, setlocale (LC_MESSAGES, NULL));
#endif
    /* End setup of GPGME */

    // Create the GPGME Context
    checkGPGError(gpgme_new (&context));
    // Set the context to textmode
    gpgme_set_textmode (context, 1);
    // Enable ASCII armor on the context
    gpgme_set_armor (context, 1);

    //Listing all keys
    gpgme_key_t key;
    gpgme_error_t err = gpgme_new (&context);
    if (!err)
    {
        err = gpgme_op_keylist_start (context, nullptr, 1);
        while (!err)
        {
            err = gpgme_op_keylist_next (context, &key);
            if (err)
                break;
            GPGkeys.push_back(key);
        }
    }
    if (gpg_err_code (err) != GPG_ERR_EOF)
    {
        fprintf (stderr, "can not list keys: %s\n", gpgme_strerror (err));
        exit (1);
    }
}

void GPGPasswdManager::readEncrypted(std::string fileName)
{
    try{
        ifstream is(fileName, ifstream::binary);
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);
        encryptedText.resize(length);
        is.read(&(encryptedText[0]), length);
    }
    catch(ifstream::failure e)
    {
        throw runtime_error("File doesn't exist");
    }
}

std::string GPGPasswdManager::dumpRecord() const
{
    gpgme_data_t in, out;
    checkGPGError(gpgme_data_new_from_mem(&in, &(encryptedText[0]), encryptedText.size()*sizeof(char),0));

    checkGPGError(gpgme_data_new(&out));
    checkGPGError(gpgme_op_decrypt(context, in, out));
    //read into a buffer
    char buffer[1024]={'\0'};
    gpgme_data_seek (out, 0, SEEK_SET);
    size_t ret = gpgme_data_read(out, buffer, 1024);
    if (ret  && ret < 1024)
    {
        return std::string{buffer};
    }
    else
    {
        std::cout<<ret<<std::endl;
        LOG::writeLogErr("Failed to decrypt\n");
        return std::string{""};
    }
    gpgme_data_release(in);
    gpgme_data_release(out);
}

void GPGPasswdManager::encrypt(const std::string & plainText)
{
    if (!recp[0])
        throw std::runtime_error("No GPG key selected, can't encrypt\n");
    gpgme_data_t in, out;
    checkGPGError(gpgme_data_new_from_mem(&in, plainText.c_str(), plainText.size(), 0));
    checkGPGError(gpgme_data_new(&out));
    checkGPGError( gpgme_op_encrypt( context, &(recp[0]), GPGME_ENCRYPT_ALWAYS_TRUST, in, out));
    gpgme_data_seek (out, 0, SEEK_SET);
    char buffer[4096]={'\0'};
    size_t ret = gpgme_data_read(out, buffer, 1024);
    if (ret  && ret < 1024)
    {
        encryptedText.resize(ret);
        std::memcpy( &(encryptedText[0]), buffer, ret);
    }
    else
    {
        LOG::writeLogErr("Failed to encrypt\n");
    }
    gpgme_data_release(in);
    gpgme_data_release(out);
}


GPGPasswdManager::~GPGPasswdManager()
{
    for (auto &key: GPGkeys)
        gpgme_key_release (key);
    gpgme_release(context);
}

void GPGPasswdManager::loadEncryptedFromMem(const char* buffer,const size_t &sz)
{
    encryptedText.resize(sz);
    std::memcpy(&(encryptedText[0]), buffer, sz);
}


void GPGPasswdManager::selectKey(size_t keyIdx)
{
    try{
        recp[0] = GPGkeys[keyIdx];
    }
    catch (const std::out_of_range& oor){
        std::cerr<<"Key selecting out of range, using nullptr\n";
        recp[0] = nullptr;
    }
}

#include <fstream>
#include <iostream>
#include "GPGPasswdManager.hpp"
using std::ifstream;
using std::runtime_error;


/**
 * @brief Constructer, initialize GPGME
 */
GPGPasswdManager::GPGPasswdManager()
{
    //gpgme_sig_mode_t sigMode = GPGME_SIG_MODE_CLEAR;
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
        std::cout<<"Listing private keys...\n";
        while (!err)
        {
            err = gpgme_op_keylist_next (context, &key);
            if (err)
                break;
            printf ("%s:", key->subkeys->keyid);
            if (key->uids && key->uids->name)
                printf (" %s", key->uids->name);
            if (key->uids && key->uids->email)
                printf (" <%s>", key->uids->email);
            putchar ('\n');
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

std::string GPGPasswdManager::dumpRecord()
{
    gpgme_data_t in, out;
    checkGPGError(gpgme_data_new_from_mem(&in, &(encryptedText[0]), encryptedText.size()*sizeof(char),1));

    checkGPGError(gpgme_data_new(&out));
    checkGPGError(gpgme_op_decrypt(context, in, out));
    //read into a buffer
    char buffer[1024]={'\0'};
    gpgme_data_seek (out, 0, SEEK_SET);
    size_t ret = gpgme_data_read(out, buffer, 1024);
    if (ret != -1 && ret < 1024)
    {
        std::cout<<"Getting plain text of "<<ret<<" bytes\n";
        return std::string{buffer};
    }
    else
    {
        LOG::writeLogErr("Failed to decrypt\n");
        return std::string{""};
    }
}

void GPGPasswdManager::encrypt()
{
    //TODO: finish this
}

GPGPasswdManager::~GPGPasswdManager()
{
    
    for (auto &key: GPGkeys)
        gpgme_key_release (key);
    gpgme_release(context);
}
void GPGPasswdManager::addRecord(const std::string &)
{
}

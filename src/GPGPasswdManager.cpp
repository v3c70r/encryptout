#include <fstream>
#include <iostream>
#include <stdexcept>
#include "GPGPasswdManager.hpp"
using std::ifstream;
using std::runtime_error;


/**
 * @brief Constructer, initialize GPGME
 */
GPGPasswdManager::GPGPasswdManager()
{
    /* Initializes gpgme */
    gpgme_check_version (NULL);

    /* Initialize the locale environment.  */
    setlocale (LC_ALL, "");
    gpgme_set_locale (NULL, LC_CTYPE, setlocale (LC_CTYPE, NULL));
#ifdef LC_MESSAGES
    gpgme_set_locale (NULL, LC_MESSAGES, setlocale (LC_MESSAGES, NULL));
#endif
    if(error)
        throw runtime_error("GPG failure");
    gpgme_set_armor(context, 1);

    /* Check OpenPGP */
    error = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
    if(error)
        throw runtime_error("GPG failure");
    error = gpgme_get_engine_info (&info);
    if(error)
        throw runtime_error("GPG failure");
    while (info && info->protocol != gpgme_get_protocol (context)) {
        info = info->next;
    }  
    fprintf (stderr, "Engine OpenPGP %s is installed at %s\n", info->version,
            info->file_name); /* And not "path" as the documentation says */


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

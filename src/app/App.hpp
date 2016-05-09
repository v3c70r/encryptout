#pragma once
#include "GPGPasswdManager.hpp"
#include <random>

/**
 * @brief A singletone class of application
 */
class App
{
public:
    static App& getInstance()
    {
        static App instance;
        return instance;
    }
    App(App const&) = delete;
    void operator=(App const&)  = delete;
    void addRecord(const std::string& record, const std::string &fileName);
    void showRecord() const{ std::cout<<manager.dumpRecord();};
    void loadEncrypt(const std::string& fileName){ manager.readEncrypted(fileName); }
    void selectKey(const size_t& idx){manager.selectKey(idx);};
    void listKeys() const {manager.listKeys();};
private:
    GPGPasswdManager manager;
    App(){
        LOG::restartLog();
    }
};


/**
 * @brief Generate random password from /dev/urandom, this function is experimental and requires polishing
 *
 * @param length length of password you want
 *
 * @return password string
 */
inline std::string generatePasswd(const size_t& length)
{
    char cmd[] ="cat /dev/urandom |  head -n 2 | tail -n 1| base64 "; 
    FILE* pipe=fopen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    fclose(pipe);
    return result.substr(result.size()-length, length);
}
inline void printPasswd()
{
    char cmd[] ="cat /dev/urandom |  head -n 2 | tail -n 1| base64 "; 
    FILE *pipe=fopen(cmd, "r");
    if (!pipe) throw std::runtime_error("Invalid pip");
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    fclose(pipe);
    std::cout<<result;
}

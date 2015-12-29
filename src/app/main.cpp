#include "App.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include "cmdline.h"
using std::ifstream;
int main(int argc, char *argv[])
{
    App& app = App::getInstance();

    cmdline::parser cmdParser;
    cmdParser.add<std::string>("cipherFile", 'c', "Cipher databse", true, "");
    cmdParser.add<std::string>("add", 'a', "Add a string as record", false);
    cmdParser.add("dump", 'd', "Dump record");
    cmdParser.add("list-keys", 'l', "List private keys");
    cmdParser.add<int>("select-key", 's', "Select a key in key array", true, 0);
    cmdParser.parse_check(argc, argv);

    std::cout<<"Cipher file "<<cmdParser.get<std::string>("cipherFile")<<std::endl;
    std::string cipherFile{cmdParser.get<std::string>("cipherFile")};

    app.loadEncrypt(cipherFile);
    app.selectKey( cmdParser.get<int>("select-key"));

    if (cmdParser.exist("add"))
        app.addRecord(cmdParser.get<std::string>("add"), cipherFile);
    if (cmdParser.exist("dump"))
        app.showRecord();



    //app.loadEncrypt("test.cipher");
    //app.listKeys();
    //app.selectKey(0);
    //app.addRecord("tsing mail.google.com acadsf", "test.cipher");
    //app.showRecord();

    return 0;
}

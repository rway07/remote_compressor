#ifndef COMMANDHANDLER_H
#define	COMMANDHANDLER_H

#include "threadHelper.h"
#include "../common/serverSideConfiguration.h"
#include "../common/network.h"
#include "compressor.h"

class server;

class commandHandler : public threadHelper
{
public:
    commandHandler(server *s, serverSideConfiguration *c);
    virtual ~commandHandler();

private:
    void handle_command();
    void command_handler();
    int command_parser(char cmd[NET_CMD_LEN]) throw(commandExecutionException);
    void enable_compressors();
    int handle_show_configuration();
    int handle_configure_name();
    int handle_configure_compressor();
    int handle_configuration_check();
    int handle_send();
    int handle_compress();
    
    network *n;
    server *s;
    serverSideConfiguration *c;
    compressorBridge *b;
};

#endif	/* COMMANDHANDLER_H */


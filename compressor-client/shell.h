#ifndef SHELL_H
#define	SHELL_H

#include "../common/common.h"
#include "../common/client.h"
#include "../common/network.h"
#include "../common/configuration.h"
#include "../common/fileHandler.h"

class shell 
{
public:
    shell(client *c, configuration *conf);
    shell(const shell& orig);
    virtual ~shell();

    int command_line() throw(notConnectedException);

private:
    int parse_command(string command) throw(commandException, notConnectedException);
    
    int check_name(string name) throw(badFileNameArgumentException);
    int check_compressor(string compressor) throw(badCompressorArgumentException);
    int configuration_check() throw(configurationIntegrityCheckException);
    int enable_compressors();
    
    int get_command_number(string command) throw(commandException);
    int params_check(int cmd, string param) throw(commandException);
    int check_compress(string param);
    int check_send(string param);
    int check_conf_name(string param);
    int check_conf_comp(string param);
    
    int do_help() throw(commandExecutionException, notConnectedException);
    int do_quit() throw(commandExecutionException, notConnectedException);
    int do_compress(string param) throw(commandExecutionException, notConnectedException);
    int do_send(string param) throw(commandExecutionException, notConnectedException);
    int do_show_conf()throw(commandExecutionException, notConnectedException);
    int do_conf_name(string param) 
        throw(nameConfigurationException, commandExecutionException, notConnectedException);
    int do_conf_comp(int type) 
        throw(compressorConfigurationException, commandExecutionException, notConnectedException);
    
    client *current_client;
    configuration *current_conf;
    string current_dir;
};

#endif	/* SHELL_H */


#ifndef SERVER_H
#define	SERVER_H

#include "../common/network.h"
#include "../common/serverSideConfiguration.h"
#include "commandHandler.h"
#include "../common/fileHandler.h"

// CONTROLLA BENE L'ALLOCAZIONE DELLA MEMORIA!!

class commandHandler;

class server : public network
{
public:
    server(configuration *base_config, int port);
    server(const server& orig);
    virtual ~server();

    int server_listen() throw(serverErrorException);
    int client_disconnect(int client_ID);
    int free_client_ID(int id);
            
private:
    int generate_client_ID();
    int configure_working_dir(string directory) throw(fileException);
    
    configuration *base_conf;
    serverSideConfiguration *conf[MAX_CLIENT];
    commandHandler *worker[MAX_CLIENT];
    int port;
    int client_socket;
    int server_socket;
    struct sockaddr_in c_address;
};

#endif	/* SERVER_H */


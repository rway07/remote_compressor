/*
 *      client.h
 *      Classe che gestisce la connessione/disconnessione dal server
 */
#ifndef COMPRESSOR_CLIENT_CLIENT_H_
#define COMPRESSOR_CLIENT_CLIENT_H_

#include "network.h"

class client : public network {
 public:
    client(char *host, int port);
    client(const client& orig);
    virtual ~client();

    int client_connect() throw(clientConnectException, badAddressException);
    int client_disconnect();
    int get_socket();

 private:
    void show_connected_state();
    char *server_address;
    int client_socket;
    int port;
};

#endif  // COMPRESSOR_CLIENT_CLIENT_H_

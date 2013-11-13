#ifndef NETWORK_H
#define	NETWORK_H

#include "../common/common.h"
#include "../common/exception.h"

enum networkResponse
{
    ERROR = -1,
    OK,
    BYE,
    CONTINUE,
};

class network 
{
public:
    network(); 
    network(const network& orig);
    virtual ~network();

    // Invio/Ricezione interi
    int send_int(int socket, int num) throw(transmissionException);
    int receive_int(int socket) throw(transmissionException, notConnectedException);
    // Invio/Ricezione comandi
    int send_command(int socket, char *command) throw(transmissionException);
    int receive_command(int socket, char *command)throw(transmissionException, notConnectedException);
    // Invio/Ricezione risposte
    int send_response(int socket, networkResponse response) throw(transmissionException);
    int receive_response(int socket) throw(transmissionException, commandExecutionException, notConnectedException);
    // Invio/Ricezione dati
    int send_data(int socket, char *buffer, int len) throw(transmissionException);
    int receive_data(int socket, char *buffer, int len) throw(transmissionException, notConnectedException);
    int receive_file(int socket, char *buffer, int len) throw(transmissionException, notConnectedException);

protected:
    struct sockaddr_in s_address;
};

#endif	/* NETWORK_H */


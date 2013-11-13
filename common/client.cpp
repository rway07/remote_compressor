/*
 *      client.cpp
 *      Implementa la classe client definita in client.h
 */
#include "client.h"

client::client(char *host, int port) {
    this->client_socket = 0;
    // Copia parametri
    this->server_address = new char[sizeof(host)];
    strcpy(this->server_address, host);
    this->port = port;
}

client::client(const client& orig) {
}

client::~client() {
    delete[] this->server_address;
}

int client::client_connect()
        throw(clientConnectException, badAddressException) {
    int ret = 0;

    if (client_socket == 0) {
        client_socket = socket(PF_INET, SOCK_STREAM, 0);
    }

    memset(&s_address, 0, sizeof(s_address));
    s_address.sin_family = AF_INET;
    s_address.sin_port = htons(port);

    ret = inet_pton(AF_INET, server_address, &s_address.sin_addr);

    if (ret == 0) {
        throw badAddressException();
        return -1;
    }

    ret = connect(client_socket, reinterpret_cast<SA*>(&s_address),
            sizeof(s_address));

    if (ret == -1) {
        throw clientConnectException();
        return -1;
    }

    client::show_connected_state();

    return 0;
}

int client::client_disconnect() {
    int ret = close(client_socket);

    return ret;
}

int client::get_socket() {
    return this->client_socket;
}

void client::show_connected_state() {
    cout << "Connesso al server " << server_address << " sulla porta "
            << port << endl;
}

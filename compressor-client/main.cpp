/*
 *      main.cpp
 *      funzione main() del client
 */

#include "paramParser.h"
#include "shell.h"
#include "../common/client.h"
#include "../common/configuration.h"
#include "../common/exception.h"

int main(int argc, char** argv) {
    int ret = 0;
    int port = 0;
    char* host = 0;

    // Welcome!!
    cout << "Remote Compressor v1.0 - Client" << endl;
    cout << endl;
    // Warnings!!
    cout << "ATTENZIONE:" << endl;
    cout << "La lunghezza massima dei comandi è di " <<
                                MAX_CMD_LEN << " caratteri" << endl;
    cout << "Il massimo numero di client è " << MAX_CLIENT << endl;
    cout << endl;

    // Controllo parametri
    paramParser parser;

    ret = parser.parse_params(argc, argv);

    if (ret != 0) {
        exit(EXIT_FAILURE);
    }

    // Copia parametri [host] e [port]
    host = new char[ADDRESS_LEN];
    parser.get_address(host);
    port = parser.get_port();

    // Istanziamento oggetto client
    client c(host, port);

    try {
        c.client_connect();
    } catch(badAddressException e) {
        e.show_error();
        exit(EXIT_FAILURE);
    } catch(clientConnectException e) {
        e.show_error();
        exit(EXIT_FAILURE);
    }

    // Creazione configurazione
    configuration client_conf;

    // Istanziamento oggetto shell solo dopo la connessione al server
    shell ui(&c, &client_conf);
    ui.command_line();

    c.client_disconnect();

    return 0;
}

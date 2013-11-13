#include "commandHandler.h"
#include "server.h"
#include "compressor.h"

commandHandler::commandHandler(server *s, serverSideConfiguration *c) {
    // Allocazione oggetti network per le funzioni di rete
    // puntatore alla configurazione corrente e alle server principale
    this->n = new network();
    this->c = c;
    this->s = s;
    this->b = new compressorBridge();
}

commandHandler::~commandHandler() {
    // Libero la memoria
    if (this->n != 0) {
        free(this->n);
    }

    if (this->c != 0) {
        free(this->c);
    }

    this->s = 0;
}

void commandHandler::enable_compressors() {
    int socket = c->get_socket();
    int i = 0;
    try {
        for (i = 2; i < SUPPORTED_COMPRESSORS; i++) {
            n->send_int(socket, c->is_compressor_supported(ZIP));
            n->receive_response(socket);
        }
    }    catch (transmissionException e) {
        e.show_error();
    }
}

// Funzione principale del thread che gestisce i comandi mandati al server

void commandHandler::handle_command() {
    char command[NET_CMD_LEN + 1];
    char response[NET_RESPONSE_LEN];
    int socket = c->get_socket();
    int client_ID = c->get_client_ID();
    string client_address = c->get_client_address();

    cout << endl;
    cout << "CLIENT " << client_address << " connesso. ID => " << client_ID << endl;

    // Connessione avvenuta, attesa comandi       
    strcpy(command, "");
    strcpy(response, "");

    try {
        // Invio client_ID al client
        n->send_int(socket, client_ID);

        // Ricezione risposta
        n->receive_response(socket);

        commandHandler::enable_compressors();
    }    catch (transmissionException e) {
        // Chiudo la connessione con il client dato che l'invio dell'ID è fallita!
        cout << "ERRORE: Trasmissione dell'ID del client fallita!! Uscita..." << endl;
        s->client_disconnect(client_ID);
        pthread_exit(NULL);
        return;
    }

    while (strcmp(NET_EXIT_CMD, command) != 0) {
        // Sezione in comune con tutti i comandi
        // Ricezione comando
        try {
            // Ricezione comando
            n->receive_command(socket, command);

            // Invio risposta
            n->send_response(socket, OK);
        }        catch (notConnectedException e) {
            e.show_error();
            break;
        }        catch (transmissionException e) {
            n->send_response(socket, ERROR);
            e.show_error();
            continue; //NON SONO MOLTO SICURO DI QUESTO
        }

        try {
            // Parsing dei comandi
            command_parser(command);
        }        catch (commandExecutionException e) {
            e.show_error();
            continue;
        }
    }

    // Disconnessione del client
    s->client_disconnect(client_ID);

    pthread_exit(NULL);
}

int commandHandler::command_parser(char cmd[NET_CMD_LEN]) throw (commandExecutionException) {
    int value = cmd[3] - '0';
    int ret = 0;
    string address = c->get_client_address();

    switch (value) {
        case 0:
            cout << "CLIENT " + address + ": eseguito comando quit" << endl;
            break;
        case 1:
            cout << "CLIENT " + address + ": eseguito comando help" << endl;
            break;
        case 2:
            cout << "CLIENT " + address + ": eseguito comando send" << endl;
            ret = handle_send();
            break;
        case 3:
            cout << "CLIENT " + address + ": eseguito comando compress" << endl;
            ret = handle_compress();
            break;
        case 4:
            cout << "CLIENT " + address + ": eseguito comando show-configuration" << endl;
            ret = handle_show_configuration();
            break;
        case 5:
            cout << "CLIENT " + address + ": eseguito comando configure-name" << endl;
            ret = handle_configure_name();
            break;
        case 6:
            cout << "CLIENT " + address + ": eseguito comando configure-compressor" << endl;
            ret = handle_configure_compressor();
            break;
        case 9:
            cout << "CLIENT " + address + ": controllo configurazione" << endl;
            ret = handle_configuration_check();
            break;
        default:
            cout << "CLIENT " + address + ": comando sconosciuto!!" << endl;
            break;
    }

    if (ret == -1) throw commandExecutionException(COMMAND_ERROR);

    return ret;
}

int commandHandler::handle_configuration_check() {
    int len = 0;
    int socket = c->get_socket();
    string name = c->get_archive_name();

    len = name.length();
    char *buffer = new char[len + 1];
    strcpy(buffer, name.c_str());

    try {
        // Invio dimensione nome archivio
        n->send_int(socket, len);

        // Ricezione risposta
        n->receive_response(socket);

        // Invio nome archivio
        n->send_data(socket, buffer, len);

        // Ricezione risposta
        n->receive_response(socket);

        // Invio indice compressore
        n->send_int(socket, c->get_compressor_index());

        // Ricezione risposta
        n->receive_response(socket);

        // Invio bye
        n->send_response(socket, BYE);
    }    catch (transmissionException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    }

    delete[] buffer;

    return 0;
}

int commandHandler::handle_show_configuration() {
    int len = 0;
    int socket = c->get_socket();
    char *buffer = new char[sizeof (c->get_archive_name()) + 1];
    strcpy(buffer, c->get_archive_name().c_str());
    len = strlen(buffer);

    try {
        // Dimensione stringa
        n->send_int(socket, len);

        // Ricezione risposta
        n->receive_response(socket);

        // Invio nome archivio
        n->send_data(socket, buffer, len);

        // Ricezione risposta
        n->receive_response(socket);

        // Invio indice compressore
        n->send_int(socket, c->get_compressor_index());

        // Ricezione risposta
        n->receive_response(socket);

        // Fine connessione
        n->send_response(socket, BYE);

        cout << "SERVER: Comando show-configuration eseguito correttamente" << endl;
    }    catch (transmissionException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    }

    // Ripulitura memoria
    delete[] buffer;

    return 0;
}

int commandHandler::handle_configure_name() {
    int len = 0;
    int socket = c->get_socket();
    char *buffer;

    try {
        // Ricezione dimensione nome archivio
        len = n->receive_int(socket);

        // Invio rispota
        n->send_response(socket, OK);

        // Ricezione nome archivio
        buffer = new char[len + 1];
        n->receive_data(socket, buffer, len);

        // Invio rispota
        n->send_response(socket, OK);

        // Ricezione fine 
        n->receive_response(socket);

        // Aggiornamento configurazione
        c->set_archive_name(buffer);

        cout << "SERVER: Comando configure-name eseguito correttamente" << endl;
    }    catch (transmissionException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    }

    delete[] buffer;

    return 0;
}

int commandHandler::handle_configure_compressor() {
    int type = 0;
    int socket = c->get_socket();

    try {
        // Ricezione indice compressore
        type = n->receive_int(socket);

        // Invio risposta
        n->send_response(socket, OK);

        // Ricezione BYE
        n->receive_response(socket);

        // Aggiornamento configurazione
        c->set_compressor_index(type);

        switch (type) {
            case GNUZIP:
                b->set_compression_algorithm(new gnuzipCompressor);
                break;
            case BZIP2:
                b->set_compression_algorithm(new bzip2Compressor);
                break;
            case ZIP:
                b->set_compression_algorithm(new zipCompressor);
                break;
            case XZ:
                b->set_compression_algorithm(new xzCompressor);
                break;
        }

        cout << "SERVER: Comando configure-compressor eseguito correttamente" << endl;
    }    catch (transmissionException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int commandHandler::handle_send() {
    int socket = c->get_socket();
    int ret = 0;
    //long len = 0;
    unsigned int len = 0;
    string file_name;
    string complete_file_name;
    char *buffer;

    try {
        // Ricezione dimensione nome file
        len = n->receive_int(socket);

        // Invio risposta
        n->send_response(socket, OK);

        // Ricezione nome file
        buffer = new char[len + 1];
        n->receive_data(socket, buffer, len);

        // Invio risposta
        n->send_response(socket, OK);

        // Ricezione dimensione file
        len = n->receive_int(socket);

        // Invio risposta
        n->send_response(socket, OK);

        // Ricezione MD5
        stringstream ss;
        ss << buffer;
        file_name = ss.str();
        complete_file_name = c->get_files_dir() + '/' + ss.str();

        //cout << "DEBUG: file output> " << complete_file_name << endl;        

        unsigned char *r_hash = new unsigned char[MD5_DIGEST_LENGTH];
        n->receive_data(socket, (char*) r_hash, MD5_DIGEST_LENGTH);

        try {
            if (fileHandler::file_exist(complete_file_name) == true) {
                unsigned char c_hash[MD5_DIGEST_LENGTH];
                fileHandler::calculate_MD5(complete_file_name, c_hash);

                if (strncmp((const char*) c_hash, (const char*) r_hash, MD5_DIGEST_LENGTH) == 0) {
                    throw sentFileAlreadyExistException();
                } else {
                    //cout << "DEBUG: MD5 non corretto!! Elimino il file" << endl;
                    fileHandler::delete_file(complete_file_name);
                }
            }
        }        catch (fileException e) {
            e.show_error();
        }

        n->send_response(socket, OK);

        // Ricezione file!!
        fileWriter file(complete_file_name, len);

        buffer = new char[NET_TRUNK];
        unsigned int i = 0;
        unsigned int next = 0;
        unsigned int last_len = 0;
        while (i <= len) {
            i += NET_TRUNK;

            if (i > len) {
                last_len = len - next;
                n->receive_file(socket, buffer, last_len);
                file.append_trunk(buffer, last_len);
            } else {
                n->receive_file(socket, buffer, NET_TRUNK);
                file.append_trunk(buffer, NET_TRUNK);
            }

            next = i;
        }

        delete[] buffer;
        file.close();

        // Verifica MD5
        unsigned char *c_hash = new unsigned char[MD5_DIGEST_LENGTH];
        fileHandler::calculate_MD5(complete_file_name, c_hash);

        cout << "SERVER: MD5 ricevuto: ";
        fileHandler::print_MD5(r_hash);
        cout << "SERVER: MD5 corrente: ";
        fileHandler::print_MD5(c_hash);

        if (strncmp((const char*) c_hash, (const char*) r_hash, MD5_DIGEST_LENGTH) == 0) {
            cout << "SERVER: MD5 corretto!!" << endl;
        } else {
            delete[] c_hash;
            delete[] r_hash;
            throw wrongMD5Exception();
            return -1;
        }

        delete[] c_hash;
        delete[] r_hash;

        // Invio risposta
        n->send_response(socket, OK);

        // Ricezione risposta
        ret = n->receive_response(socket);

        if (ret == 0) {
            cout << "SERVER: ricevuto il file " << fileHandler::extract_file_name(complete_file_name);
            cout << " dal client " << c->get_client_address() << endl;
        }
    }    catch (transmissionException e) {
        // Qui dovrei mandare una segnalazione d'errore!
        e.show_error();
        delete[] buffer;
        return -1;
    }    catch (fileWriterException e) {
        n->send_response(socket, ERROR);
        e.show_error();
        delete[] buffer;
        return -1;
    }    catch (sentFileAlreadyExistException e) {
        e.show_error();
        n->send_response(socket, CONTINUE);
        return 0;
    }    catch (wrongMD5Exception e) {
        n->send_response(socket, ERROR);
        e.show_error();
        return -1;
    }

    return 0;
}

int commandHandler::handle_compress() {
    int socket = c->get_socket();
    long len = 0;
    string file_name;
    string client_address = c->get_client_address();
    char *buffer;
    int ret = 0;

    cout << "SERVER: compressione richiesta dal client " << c->get_client_address() << endl;

    // Compressione
    ret = b->compress(c);

    if (ret != 0) {
        n->send_response(socket, ERROR);
        cout << "ERRORE: Errore nella creazione dell'archivio!" << endl;
        return -1;
    }

    file_name = b->get_archive_name();
    cout << "SERVER: Creato archivio " << file_name << endl;

    try {
        file_name = c->get_archive_dir() + '/' + file_name;

        if (fileHandler::file_exist(file_name) == false) {
            n->send_response(socket, ERROR);
            cout << "ERRORE: Il file richiesto non è disponibile!!" << endl;
            return -1;
        }
    }    catch (fileException e) {
        e.show_error();
        return -1;
    }

    try {
        // Invio OK
        n->send_response(socket, OK);

        // Invio dimensione file
        len = fileHandler::get_file_len(file_name);
        n->send_int(socket, len);

        // Ricezione risposta
        n->receive_response(socket);

        // Calcolo MD5
        unsigned char hash[MD5_DIGEST_LENGTH];
        fileHandler::calculate_MD5(file_name, hash);

        // Invio MD5
        n->send_data(socket, (char*) hash, MD5_DIGEST_LENGTH);

        n->receive_response(socket);

        cout << "SERVER: Hash MD5: ";
        fileHandler::print_MD5(hash);

        // Invio file
        fileReader file(file_name, len);

        long i = 0;
        long next = 0;
        long last_len = 0;
        buffer = new char[NET_TRUNK];
        while (i <= len) {
            i += NET_TRUNK;

            if (i > len) {
                last_len = len - next;
                file.read_trunk(buffer, last_len);
                n->send_data(socket, buffer, last_len);
            } else {
                file.read_trunk(buffer, NET_TRUNK);
                n->send_data(socket, buffer, NET_TRUNK);
            }

            next = i;
        }

        delete[] buffer;
        file.close();

        // Ricezione risposta
        n->receive_response(socket);

        // Fine trasmissione
        n->send_response(socket, BYE);

        fileHandler::delete_file(file_name);

        cout << "SERVER: spedito il file " << fileHandler::extract_file_name(file_name);
        cout << " al client " << client_address << endl;
    }    catch (transmissionException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    }    catch (fileReaderException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    }

    return 0;
}

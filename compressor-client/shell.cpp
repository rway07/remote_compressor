#include "shell.h"

shell::shell(client *c, configuration *conf) {
    this->current_client = c;
    this->current_conf = conf;
}

shell::shell(const shell& orig) {
}

shell::~shell() {
    this->current_client = 0;
    this->current_conf = 0;
}

int shell::enable_compressors() {
    int socket = current_client->get_socket();
    int ret = 0, i = 0;
    try {
        for (i = 2; i < SUPPORTED_COMPRESSORS; i++) {
            ret = current_client->receive_int(socket);
            current_client->send_response(socket, OK);

            if (ret)
                current_conf->enable_compressor(i);
        }
    } catch(transmissionException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int shell::command_line() throw(notConnectedException) {
    string cmd;
    int ret = 0;
    int client_ID = 0;

    try {
        // Ricezione client_ID dal server
        client_ID = current_client->receive_int(current_client->get_socket());

        // Invio risposta
        current_client->send_response(current_client->get_socket(), OK);

        current_conf->set_client_ID(client_ID);

        shell::enable_compressors();
    } catch(transmissionException e) {
        cout << "ERRORE: Errore nella ricezione del cliend_ID!! Uscita..."
                << endl;
        return -1;
    }

    do {
        cout << "remote-compressor> ";
        cmd.clear();
        getline(cin, cmd);

        try {
            ret = parse_command(cmd);
        } catch(commandException e) {
            e.show_error();
        } catch(commandExecutionException e) {
            e.show_error();
        }
    } while ((ret != EXIT_COMMAND_NOTIFY) && (ret != NOT_CONNECTED_NOTIFY));

    return 0;
}

int shell::configuration_check() throw(configurationIntegrityCheckException) {
    string current_archive_name = current_conf->get_archive_name();
    int current_index = current_conf->get_compressor_index();
    int socket = current_client->get_socket();
    char *cmd = new char[NET_CMD_LEN];
    int len = 0, index = 0;
    char *buffer;

    try {
        // Invio codifica comando
        strcpy(cmd, NET_CONF_CHECK_CMD);
        current_client->send_command(socket, cmd);

        // Ricezione conferma
        current_client->receive_response(socket);

        // Ricezione configurazione
        // Ricezione dimensione nome
        len = current_client->receive_int(socket);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione nome
        buffer = new char[len + 1];
        current_client->receive_data(socket, buffer, len);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione indice compressore
        index = current_client->receive_int(socket);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione Bye
        current_client->receive_response(socket);

        // Controllo configurazione
        // Controllo nome archivio
        cout << "Controllo configurazione...";
        if (strcmp(buffer, current_archive_name.c_str()) != 0) {
            cout << "NAME FIX!";
            current_conf->set_archive_name(buffer);
        }

        if (index != current_index) {
            cout << "INDEX FIX!";
            current_conf->set_compressor_index(index);
        }

        delete[] cmd;
        delete[] buffer;

        cout << "OK" << endl;
    } catch(transmissionException e) {
        e.show_error();
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    return 0;
}

int shell::get_command_number(string command) throw(commandException) {
    int select = 0;

    if (command.compare(EXIT_CMD) == 0) {
        select = NET_EXIT_CMD[3] - '0';
    } else if (command.compare(HELP_CMD) == 0) {
        select = NET_HELP_CMD[3] - '0';
    } else if (command.compare(SHOW_CFG_CMD) == 0) {
        select = NET_SHOW_CFG_CMD[3] - '0';
    } else if (command.compare(CFG_NAME_CMD) == 0) {
        select = NET_CFG_NAME_CMD[3] - '0';
    } else if (command.compare(CFG_COMP_CMD) == 0) {
        select = NET_CFG_COMP_CMD[3] - '0';
    } else if (command.compare(SEND_CMD) == 0) {
        select = NET_SEND_CMD[3] - '0';
    } else if (command.compare(COMPRESS_CMD) == 0) {
        select = NET_COMPRESS_CMD[3] - '0';
    } else {
        throw commandException(UNKNOWN_COMMAND);
    }
    return select;
}

int shell::params_check(int cmd, string param) throw(commandException) {
    switch (cmd) {
        case 0:
        case 1:
        case 4:
            if (param.length() > 0) {
                throw commandException(BAD_COMMAND);
                return -1;
            }
            break;
        case 2:
        case 3:
        case 5:
        case 6:
            if (param.length() == 0) {
                throw commandException(INCOMPLETE_COMMAND);
                return -1;
            }
            break;
    }

    return 0;
}

int shell::check_conf_name(string param) {
    try {
        shell::check_name(param);
    } catch(badFileNameArgumentException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int shell::check_conf_comp(string param) {
    int type = 0;

    try {
        type = shell::check_compressor(param);
    } catch(badCompressorArgumentException e) {
        e.show_error();
        return -1;
    }

    return type;
}

int shell::check_compress(string param) {
    int sent_files = current_conf->get_sent_files();
    int sent_files_at_compress = current_conf->get_sent_files_at_compress();

    try {
        // Controllo numero di files inviati
        if (sent_files == 0) {
            throw commandExecutionException(NO_SENT_FILES);
            return -1;
        }

        if ((sent_files_at_compress == sent_files)
                && (current_conf->get_dirty_flag() == false)) {
            throw commandExecutionException(ALREADY_COMPRESSED_ARCHIVE);
            return -1;
        }

        // Controllo se il parametro passato corrisponde ad una cartella
        bool ret = fileHandler::is_directory(param);
        if (ret == false) {
            throw badDirectoryArgumentException();
            return -1;
        }

        // Controllo se ho i permessi di scrittura sulla cartella
        ret = fileHandler::got_permissions(param);
        if (ret == false) {
            // Eccezione
            return -1;
        }

        // Creazione cartella per la ricezione degli archivi
        param = current_conf->get_recv_folder_name(param);
        fileHandler::create_directory(param);

        // Controllo integrità configurazione
        shell::configuration_check();
    } catch(fileException e) {
        e.show_error();
        return -1;
    } catch(badDirectoryArgumentException e) {
        e.show_error();
        return -1;
    } catch(configurationIntegrityCheckException e) {
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int shell::check_send(string param) {
    string name;

    try {
        if (fileHandler::is_directory(param)) {
            throw fileException(ENOENT);
            return -1;
        } else if ((param[0] == '/') && (param[1] != '/')) {
            if (fileHandler::file_exist(param) == false) {
                throw fileException(errno);
            }
        } else {
            name = this->current_dir + '/' + param;
            if (fileHandler::file_exist(name) == false)
                throw fileException(errno);
        }
    } catch(fileException e) {
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int shell::parse_command(string command)
        throw(commandException, notConnectedException) {
    if (command.length() > MAX_CMD_LEN) {
        throw commandException(OUT_OF_LEN_COMMAND);
        return -1;
    }

    stringstream ss(command);
    string params[MAX_CMD_TOKENS];
    string temp;
    int select = 0;
    int ret = 0;
    int n = 0;

    while (std::getline(ss, temp, ' ')) {
        n++;

        if (n > MAX_CMD_TOKENS) {
            throw commandException(BAD_COMMAND);
            return -1;
        }

        params[n - 1] = temp;
    }

    try {
        select = get_command_number(params[CMD_POS]);
    } catch(commandException e) {
        e.show_error();
        return -1;
    }

    try {
        temp = params[PARAM_POS];
        params_check(select, temp);

        switch (select) {
            case 0:
                do_quit();
                return EXIT_COMMAND_NOTIFY;
                break;
            case 1:
                do_help();
                break;
            case 2:
                ret = check_send(temp);
                if (ret == 0) {
                    do_send(temp);
                    current_conf->increase_sent_files();
                }
                break;
            case 3:
                ret = check_compress(temp);
                if (ret == 0) {
                    do_compress(temp);
                    current_conf->reset_dirty_flag();
                }
                break;
            case 4:
                do_show_conf();
                break;
            case 5:
                ret = check_conf_name(temp);
                try {
                    if (ret == 0)
                        do_conf_name(temp);
                } catch(nameConfigurationException e) {
                    e.show_error();
                    return -1;
                }
                break;
            case 6:
                ret = check_conf_comp(temp);
                try {
                    if (ret != -1)
                        do_conf_comp(ret);
                } catch(compressorConfigurationException e) {
                    e.show_error();
                    return -1;
                }
                break;
            default:
                break;
        }
    } catch(notConnectedException e) {
        e.show_error();
        return NOT_CONNECTED_NOTIFY;
    }

    return 0;
}

int shell::check_compressor(string compressor)
        throw(badCompressorArgumentException) {
    int ret = 0;

    ret = current_conf->check_compressor_argument(compressor);

    if (ret == -1) {
        throw badCompressorArgumentException();
        return -1;
    }

    return ret;
}

int shell::check_name(string name) throw(badFileNameArgumentException) {
    if (name.length() > MAX_FILENAME_LEN) {
        throw badFileNameArgumentException();
        return -1;
    }

    if (name.find('*') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find(':') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('\\') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('/') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('<') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('>') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('|') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('"') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }
    if (name.find('?') != string::npos) {
        throw badFileNameArgumentException();
        return -1;
    }

    return 0;
}

int shell::do_help() throw(commandExecutionException, notConnectedException) {
    char cmd[CMD_REAL_LEN];
    int socket = current_client->get_socket();
    int i = 0;

    strcpy(cmd, NET_HELP_CMD);
    try {
        current_client->send_command(socket, cmd);

        cout << "remote-compressor - lista comandi" << endl;
        cout << "*) configure-compressor [compressor]";

        for (i = 0; i < SUPPORTED_COMPRESSORS; i++) {
            cout << " " << current_conf->get_compressor_name(i);
        }
        cout << endl;

        cout << "*) configure-name [archive_name]" << endl;
        cout << "*) show-configuration" << endl;
        cout << "*) send [local_file]" << endl;
        cout << "*) compress [local_path]" << endl;
        cout << "*) help" << endl;
        cout << "*) quit" << endl;
        cout << endl;

        current_client->receive_response(socket);
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    } catch(transmissionException e) {
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

int shell::do_quit() throw(commandExecutionException, notConnectedException) {
    char cmd[CMD_REAL_LEN];
    int socket = current_client->get_socket();

    strcpy(cmd, NET_EXIT_CMD);
    try {
        current_client->send_command(socket, cmd);
        current_client->receive_response(socket);
    } catch(transmissionException e) {
        e.show_error();
    } catch(commandExecutionException e) {
        e.show_error();
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    cout << "Bye Bye" << endl;

    return 0;
}

int shell::do_send(string param)
        throw(commandExecutionException, notConnectedException) {
    char cmd[CMD_REAL_LEN];
    int socket = current_client->get_socket();
    int len = 0;
    string file_name;

    strcpy(cmd, NET_SEND_CMD);
    try {
        // Invio comando
        current_client->send_command(socket, cmd);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio dimensione nome file
        string fname = fileHandler::extract_file_name(param);
        len = fname.length();
        current_client->send_int(socket, len);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio nome file
        char *name = new char[len + 1];
        strcpy(name, fname.c_str());
        current_client->send_data(socket, name, len);
        delete[] name;

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio dimensione file
        len = fileHandler::get_file_len(param);

        current_client->send_int(socket, len);

        // Ricezione conferma
        current_client->receive_response(socket);

        // Mandiamo anche l'md5!!
        unsigned char hash[MD5_DIGEST_LENGTH];
        fileHandler::calculate_MD5(param, hash);

        current_client->send_data(socket,
                reinterpret_cast<char*>(hash), MD5_DIGEST_LENGTH);

        current_client->receive_response(socket);

        cout << "Hash MD5: ";
        fileHandler::print_MD5(hash);

        // Invio file!!!
        fileReader file(param, len);

        int i = 0;
        int next = 0;
        int last_len = 0;
        char *buffer = new char[NET_TRUNK];
        while (i <= len) {
            i += NET_TRUNK;

            if (i > len) {
                last_len = len - next;
                file.read_trunk(buffer, last_len);
                current_client->send_data(socket, buffer, last_len);
            } else {
                file.read_trunk(buffer, NET_TRUNK);
                current_client->send_data(socket, buffer, NET_TRUNK);
            }

            next = i;
        }

        delete[] buffer;
        file.close();

        // Ricezione conferma
        current_client->receive_response(socket);

        // Fine comunicazione
        current_client->send_response(socket, BYE);

        cout << "File " << fname << " mandato con successo." << endl;
    } catch(transmissionException e) {
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    } catch(fileException e) {
        e.show_error();
        throw commandExecutionException(COMMAND_ERROR);
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    return 0;
}

int shell::do_compress(string param)
        throw(commandExecutionException, notConnectedException) {
    char cmd[CMD_REAL_LEN];
    int socket = current_client->get_socket();
    long len = 0;  // Before long len
    char *buffer;

    strcpy(cmd, NET_COMPRESS_CMD);
    try {
        // Invio comando
        current_client->send_command(socket, cmd);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Ricezione corretta esecuzione comando tar
        current_client->receive_response(socket);

        // Ricezione dimensione file
        len = current_client->receive_int(socket);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione MD5
        unsigned char *r_hash = new unsigned char[MD5_DIGEST_LENGTH];
        current_client->receive_data(socket,
                reinterpret_cast<char*>(r_hash), MD5_DIGEST_LENGTH);

        current_client->send_response(socket, OK);

        // Ricezione file
        param = current_conf->get_recv_folder_name(param);

        string file_name = param + '/' + current_conf->get_archive_name() +
         current_conf->get_compressor_ext(current_conf->get_compressor_index());

        // Cancelliamo i vecchi file
        try {
            if (fileHandler::file_exist(file_name) == true) {
                fileHandler::delete_file(file_name);
            }
        } catch(fileException e) {
            e.show_error();
        }

        fileWriter file(file_name, len);

        buffer = new char[NET_TRUNK];
        // Queste variabili prima erano long
        int i = 0;
        int next = 0;
        int last_len = 0;
        while (i <= len) {
            i += NET_TRUNK;

            if (i > len) {
                last_len = len - next;
                current_client->receive_file(socket, buffer, last_len);
                file.append_trunk(buffer, last_len);
            } else {
                current_client->receive_file(socket, buffer, NET_TRUNK);
                file.append_trunk(buffer, NET_TRUNK);
            }

            next = i;
        }

        delete[] buffer;
        file.close();

        unsigned char *c_hash = new unsigned char[MD5_DIGEST_LENGTH];
        fileHandler::calculate_MD5(file_name, c_hash);

        cout << "Hash MD5 ricevuto: ";
        fileHandler::print_MD5(r_hash);

        cout << "Hash MD5 corrente: ";
        fileHandler::print_MD5(c_hash);

        if (strncmp((const char*) c_hash, (const char*) r_hash,
                MD5_DIGEST_LENGTH) == 0) {
            cout << "MD5 corretto!!" << endl;
        } else {
            delete[] c_hash;
            delete[] r_hash;
            throw wrongMD5Exception();
            return -1;
        }

        delete[] c_hash;
        delete[] r_hash;

        // Invio risposta
        current_client->send_response(socket, OK);

        // Fine trasmissione
        current_client->receive_response(socket);

        // Aggiornamento numero files spediti al momento della compressione
        current_conf->set_sent_files_at_compress();

        cout << "Archivio " << fileHandler::extract_file_name(file_name)
                << " ricevuto con successo." << endl;
    } catch(transmissionException e) {
        e.show_error();
        return -1;
    } catch(fileWriterException e) {
        e.show_error();
        delete[] buffer;
        return -1;
    } catch(wrongMD5Exception e) {
        current_client->send_response(socket, ERROR);
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }
    
    return 0;
}

int shell::do_conf_comp(int type) throw(compressorConfigurationException,
        commandExecutionException, notConnectedException) {
    char cmd[CMD_REAL_LEN];
    int socket = current_client->get_socket();
    
    try {
        // Invio comando
        strcpy(cmd, NET_CFG_COMP_CMD);
        current_client->send_command(socket, cmd);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio tipo compressore
        current_client->send_int(socket, type);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Fine trasmissione dati
        current_client->send_response(socket, BYE);

        // Aggiornamento configurazione
        current_conf->set_compressor_index(type);

        cout << "Compressore configurato con successo." << endl;
    } catch(transmissionException e) {
        e.show_error();
        throw compressorConfigurationException();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    return 0;
}

int shell::do_conf_name(string param)
        throw(nameConfigurationException, commandExecutionException,
                notConnectedException) {
    int len = 0;
    int socket = current_client->get_socket();
    char cmd[CMD_REAL_LEN];

    try {
        // Invio comando
        strcpy(cmd, NET_CFG_NAME_CMD);
        current_client->send_command(socket, cmd);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio dimensione nome
        len = param.length();
        current_client->send_int(socket, len);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Invio nome archvio
        char *buffer = new char[len + 1];
        strcpy(buffer, param.c_str());
        current_client->send_data(socket, buffer, len);

        // Ricezione risposta
        current_client->receive_response(socket);

        // Fine trasmissione dati
        current_client->send_response(socket, BYE);

        // Solo alla fine viene cambiata la configurazione localmente
        current_conf->set_archive_name(buffer);

        // Free memory
        delete[] buffer;

        cout << "Nome configurato con successo." << endl;
    } catch(transmissionException e) {
        e.show_error();
        throw nameConfigurationException();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    return 0;
}

int shell::do_show_conf() throw(commandExecutionException,
                                        notConnectedException) {
    char cmd[CMD_REAL_LEN];
    char *buffer;
    int len = 0, index = 0;
    int socket = current_client->get_socket();

    try {
        // Invio codifica comando
        strcpy(cmd, NET_SHOW_CFG_CMD);
        current_client->send_command(socket, cmd);

        // Ricezione conferma
        current_client->receive_response(socket);

        // Ricezione configurazione
        // Ricezione dimensione nome
        len = current_client->receive_int(socket);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione nome
        buffer = new char[len + 1];
        current_client->receive_data(socket, buffer, len);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione indice compressore
        index = current_client->receive_int(socket);

        // Invio risposta
        current_client->send_response(socket, OK);

        // Ricezione Bye
        current_client->receive_response(socket);
    } catch(transmissionException e) {
        current_client->send_response(socket, ERROR);
        e.show_error();
        return -1;
    } catch(commandExecutionException e) {
        e.show_error();
        return -1;
    } catch(notConnectedException e) {
        throw notConnectedException();
        return -1;
    }

    // Solo se non ci sono stati errori nel comando viene aggiornata la
    // configurazione locale con quella del server
    // Aggiornamento configurazione locale
    current_conf->set_archive_name(buffer);
    current_conf->set_compressor_index(index);

    // Stampa configurazione
    cout << "Configurazione corrente" << endl;
    cout << "Nome archivio > " << buffer << endl;
    cout << "Compressore > " << current_conf->get_compressor_name(index);
    cout << endl;

    delete[] buffer;

    return 0;
}

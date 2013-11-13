/*
 *      paramParser.cpp
 *      Implementazione della classe paramParser
 */

#include "paramParser.h"

paramParser::paramParser() {
    this->port = 0;
    this->address = new char[ADDRESS_LEN];
}

paramParser::paramParser(const paramParser& orig) {
}

paramParser::~paramParser() {
    delete[] this->address;
}

int paramParser::check_ip_address(char* address)
                        throw(badAddressParamException) {
    struct sockaddr_in sa;
    int ret = inet_pton(AF_INET, address, &sa.sin_addr);

    if (ret == 0) {
        throw badAddressParamException();
        return -1;
    }

    strcpy(this->address, address);
    //snprintf(this->address, sizeof(address), address);
    
    return ret;
}

int paramParser::check_port(char* port) throw(badPortParamException) {
    int temp = 0;
    int i = 0;
    int p = 0;

    int len = strlen(port);
    for (i = 0; i < len; i++) {
        temp = static_cast<int>(port[i]) - '0';

        if ((temp < 0) || (temp > 9)) {
            throw badPortParamException();
            return -1;
        }

        p = p + (temp * static_cast<int>(pow(10, (len - i - 1))));
    }

    if ((p < MIN_PORT) || (p > MAX_PORT)) {
        throw badPortParamException();
        return -1;
    }

    this->port = p;

    return 0;
}

int paramParser::check_params_number(int argc)
                throw(badParamsNumberException, noParamsClientException) {
    if (argc == 1) {
        throw noParamsClientException();
    } else {
        throw badParamsNumberException();
    }

    return -1;
}

int paramParser::get_port() {
    return this->port;
}

void paramParser::get_address(char* address) {
    strcpy(address, this->address);
}

int paramParser::parse_params(int argc, char** argv) {
    // Controllo numero parametri
    if (argc != 3) {
        try {
            check_params_number(argc);
        } catch(noParamsClientException e) {
            e.show_error();
            return -1;
        } catch(badParamsNumberException e) {
            e.show_error();
            return -1;
        }
    }

    // Controllo parametro [host]
    char *address = reinterpret_cast<char*>
                (malloc(sizeof(argv[CLIENT_PARAM_HOST_POS])));
    strcpy(address, argv[CLIENT_PARAM_HOST_POS]);

    try {
        check_ip_address(address);
    } catch(badAddressParamException e) {
        e.show_error();
        return -1;
    }

    // Controllo parametro [port]
    try {
        check_port(argv[CLIENT_PARAM_PORT_POS]);
    } catch(badPortParamException e) {
        e.show_error();
        return -1;
    }

    return 0;
}

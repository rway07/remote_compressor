#ifndef PARAMPARSER_H
#define	PARAMPARSER_H

#include "../common/common.h"
#include "../common/exception.h"

class paramParser 
{
public:
    paramParser();
    paramParser(const paramParser& orig);
    virtual ~paramParser();
    
    int parse_params(int argc, char** argv);
    int get_port();
    void get_address(char *address);
    
private:
    int check_ip_address(char *address) throw(badAddressParamException);
    int check_port(char *port) throw(badPortParamException);
    int check_params_number(int argc) throw(badParamsNumberException, noParamsClientException);
    
    int port;
    char *address;
};

#endif	/* PARAMPARSER_H */


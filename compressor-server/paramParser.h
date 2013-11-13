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
    
private:
    int check_port(char *port) throw(badPortParamException);
    int check_params_number(int argc) throw(badParamsNumberException, noParamsServerException);
    
    int port;
};

#endif	/* PARAMPARSER_H */


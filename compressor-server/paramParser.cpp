#include "paramParser.h"

paramParser::paramParser() 
{
    this->port = 0;
}

paramParser::paramParser(const paramParser& orig) 
{
}

paramParser::~paramParser() 
{
}

int paramParser::check_port(char* port) throw(badPortParamException)
{
    int temp = 0;
    int i = 0;
    int p = 0;
    
    int len = strlen(port);
    for (i=0; i<len; i++)
    {
        temp = (int)port[i] - '0';
        
        if ((temp < 0) || (temp > 9))
        {
            throw badPortParamException();
            return -1;
        }
        
        p = p + (temp * (int)pow(10, (len-i-1)));
    }
    
    if ((p < MIN_PORT) || (p > MAX_PORT))
    {
        throw badPortParamException();
        return -1;
    }

    this->port = p;
    
    return 0;
}

int paramParser::check_params_number(int argc) throw(badParamsNumberException, noParamsServerException)
{
    if (argc == 1)
    {
        throw noParamsServerException();
    } else
    {
        throw badParamsNumberException();
    }
    
    return -1;
}

int paramParser::get_port()
{
    return this->port;
}

int paramParser::parse_params(int argc, char** argv)
{    
    // Controllo numero parametri
    if (argc != 2)
    {
        try
        {
            check_params_number(argc);            
        }
        catch (noParamsServerException e)
        {
            e.show_error();
            return -1;
        }
        catch (badParamsNumberException e)
        {
            e.show_error();
            return -1;
        }
    }
    // Controllo parametro [port]
    try
    {
        check_port(argv[SERVER_PARAM_PORT_POS]);
    }
    catch (badPortParamException e)
    {
        e.show_error();
        return -1;
    }
    
       
    return 0;    
}

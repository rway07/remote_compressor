#include "../common/common.h"
#include "server.h"
#include "paramParser.h"
//#include "../common/serverSideConfiguration.h"

int main(int argc, char** argv) 
{
    int ret = 0, port = 0;
    
    // Welcome!!
    cout << "Remote Compressor v1.0 - Server" << endl;
    
    // Controllo requisiti
    configuration *base_conf = new configuration();
    
    if (base_conf->check_compressors() == false) 
        exit(EXIT_FAILURE);
    
    // Parsing parametri
    paramParser parser;
    
    ret = parser.parse_params(argc, argv);
    
    if (ret != 0)
    {
        exit(EXIT_FAILURE);
    }
    
    port = parser.get_port();
    parser.~paramParser();
    
    // Check for writable folder!!
    try
    {
        fileHandler::got_permissions(fileHandler::get_working_dir());
    }
    catch (fileException e)
    {
        cout << "ERRORE: Il server non ha i permessi di scrittura sulla directory corrente!! Uscita forzata!!" << endl;
        e.show_error();
        exit(EXIT_FAILURE); 
    }
    
    // Listening
    server s(base_conf, port);
    
    try
    {
        s.server_listen();
    } 
    catch (serverErrorException e)
    {
        e.show_error();
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

#include "server.h"
#include "compressor.h"

server::server(configuration *base_config, int port) 
{
    this->port = port;
    this->client_socket = 0;
    this->server_socket = 0;
    this->base_conf = base_config;
    
    int i = 0;
    for (i = 0; i < MAX_CLIENT; i++)
    {
        this->conf[i] = 0;
    }
}

server::server(const server& orig) {}

server::~server() {}

int server::configure_working_dir(string directory) throw(fileException)
{
    if (fileHandler::create_directory(directory) == -1)
    {
        try
        {
            fileHandler::delete_directory(directory);
            fileHandler::create_directory(directory);
        }
        catch (fileException e)
        {
            e.show_error();
            return -1;
        }
    }   
    return 0;
}

int server::server_listen() throw(serverErrorException)
{
    int ret=0;
    
    // Creazione socket
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    if (server_socket == -1)
    {
        throw serverErrorException(SOCKET);
        return -1;
    }
   
    memset(&s_address, 0, sizeof(s_address));
    this->s_address.sin_family = AF_INET;
    this->s_address.sin_port = htons(this->port);
    this->s_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    ret = bind(server_socket, (SA *)&this->s_address, sizeof(this->s_address));
    
    if (ret == -1)
    {
        throw serverErrorException(BIND);
        return -1;
    }
    
    ret = listen(server_socket, MAX_PENDING_CONNECTIONS);
    
    if (ret == -1)
    {
        throw serverErrorException(LISTEN);
        return -1;
    }
      
    unsigned int len = 0;
    
    cout << "In attesa di connessioni..." << endl;
    for(;;)
    {    
        len = sizeof(c_address);
        client_socket = accept(server_socket, (SA *)&c_address, &len);
    
        if (client_socket == -1)
        {
            throw serverErrorException(ACCEPT);
            break;
        }
        
        char *address = inet_ntoa(c_address.sin_addr);
        int client_ID = server::generate_client_ID();        
        conf[client_ID] = new serverSideConfiguration(base_conf, client_socket, client_ID, address);
        
        try
        {
            ret = conf[client_ID]->set_working_dir();    
            server::configure_working_dir(conf[client_ID]->get_files_dir());
            server::configure_working_dir(conf[client_ID]->get_archive_dir());
        }
        catch (fileException e)
        {
            e.show_error();
            throw serverErrorException(WORKINGDIR);
            return -1;    
        }
        
        worker[client_ID] = new commandHandler(this, conf[client_ID]);        
        worker[client_ID]->thread_start();      
    }

    //close(c_socket);
    close(server_socket);
    
    return 0;
}

int server::client_disconnect(int client_ID)
{
    cout << "SERVER: client ID " << client_ID;
    cout << ":" << conf[client_ID]->get_client_address() << " chiude la connessione." << endl;
    
    close(conf[client_ID]->get_socket());
    
    string archive_dir = conf[client_ID]->get_archive_dir();
    string files_dir = conf[client_ID]->get_files_dir();
    
    server::free_client_ID(client_ID);
    
    try
    {
        fileHandler::delete_directory(archive_dir);
        fileHandler::delete_directory(files_dir);
    }
    catch (fileException e)
    {
        e.show_error();
        return -1;
    }
    
    return 0;
}

int server::generate_client_ID()
{
    int i = 0;
    
    for (i = 0; i < MAX_CLIENT; i++)
    {
        if (conf[i] == 0) return i;
    }

    return -1;
}

int server::free_client_ID(int id)
{
    // Usiamo anche qui le eccezioni??
    if (conf[id] == 0) 
    {
        //cout << "DEBUG: Client ID già libero!!" << endl;
        return -1;
    }
    conf[id]->~serverSideConfiguration();
    conf[id] = 0;
    
    worker[id]->~commandHandler();
    worker[id] = 0;
    
    return 0;
}

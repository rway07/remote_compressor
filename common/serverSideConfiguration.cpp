#include "serverSideConfiguration.h"

serverSideConfiguration::serverSideConfiguration() {}

serverSideConfiguration::serverSideConfiguration(const configuration& orig) : configuration(orig)
{    
}

serverSideConfiguration::serverSideConfiguration(const configuration *base_conf, int socket, int ID, char *address)
        : configuration(base_conf)
{    
    this->client_socket = socket;
    this->client_ID = ID;
    this->client_address = new char[ADDRESS_LEN +1];
    strcpy(this->client_address, address);
    this->base_directory.clear();
    this->files_directory.clear();
    this->archive_directory.clear();
}

int serverSideConfiguration::get_socket()
{
    return this->client_socket;
}

int serverSideConfiguration::get_client_ID()
{
    return this->client_ID;
}

serverSideConfiguration::~serverSideConfiguration()
{
    if (this->client_address != 0)
    {
        delete[] this->client_address;
    }
}

string serverSideConfiguration::get_client_address()
{
    return this->client_address;
}

int serverSideConfiguration::set_working_dir() throw(fileException)
{
    stringstream ss;
    ss << client_ID;
    
    try
    {
        this->base_directory = fileHandler::get_working_dir();
        this->archive_directory_name = "archivi_client" + ss.str();
        this->files_directory_name = "files_client" + ss.str();
        this->archive_directory = this->base_directory + '/' + this->archive_directory_name;
        this->files_directory = this->base_directory + '/' + this->files_directory_name;
    }
    catch (fileException e)
    {
        throw fileException(errno);
        return -1;
    }
    
    return 0;
}

string serverSideConfiguration::get_base_dir()
{
    return this->base_directory;
}

string serverSideConfiguration::get_archive_dir()
{
    return this->archive_directory;
}

string serverSideConfiguration::get_archive_dir_name()
{
    return this->archive_directory_name;
}

string serverSideConfiguration::get_files_dir()
{
    return this->files_directory;
}

string serverSideConfiguration::get_files_dir_name()
{
    return this->files_directory_name;
}

#ifndef SERVERSIDECONFIGURATION_H
#define	SERVERSIDECONFIGURATION_H

#include "../common/common.h"
#include "../common/configuration.h"
#include "fileHandler.h"

class serverSideConfiguration : public configuration
{
public:
    serverSideConfiguration();
    serverSideConfiguration(const configuration& orig);
    serverSideConfiguration(const configuration *base_conf, int socket, int ID, char *address);
    ~serverSideConfiguration();
    
    int get_client_ID();
    int get_socket();
    string get_client_address();
    int set_working_dir() throw(fileException);
    string get_base_dir();
    string get_files_dir();
    string get_archive_dir();
    string get_files_dir_name();
    string get_archive_dir_name();
    
private:
    int client_socket;
    char* client_address;
    string files_directory;
    string archive_directory;
    string base_directory;
    string files_directory_name;
    string archive_directory_name;
};

#endif	/* SERVERSIDECONFIGURATION_H */

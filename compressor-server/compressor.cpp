#include "compressor.h"

compressor::compressor() {}

compressor::~compressor(){}

string compressor::get_archive_name()
{
    return this->current_file_name;
}

compressorBridge::compressorBridge()
{
    this->algorithm = new gnuzipCompressor();
}

void compressorBridge::set_compression_algorithm(compressor *alg)
{
    if (this->algorithm != NULL)
        delete this->algorithm;
    
    this->algorithm = alg;
}

int compressorBridge::compress(serverSideConfiguration *c)
{
    return algorithm->compress(c);
}

string compressorBridge::get_archive_name()
{
    return algorithm->get_archive_name();
}

gnuzipCompressor::~gnuzipCompressor(){}
bzip2Compressor::~bzip2Compressor(){}
zipCompressor::~zipCompressor(){}
xzCompressor::~xzCompressor(){}

int gnuzipCompressor::compress(serverSideConfiguration* c)
{
    int ret = 0;
    string command;
    char *cmd;
    
    current_file_name = c->get_archive_name() + GNUZIP_EXT;
    command = GNUZIP_COMMAND + c->get_archive_dir() + '/' + current_file_name + " " + c->get_files_dir_name();

    cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
    
    delete[] cmd;
    
    return ret;
}

int bzip2Compressor::compress(serverSideConfiguration* c)
{
    int ret = 0;
    string command;
    char *cmd;
    
    current_file_name = c->get_archive_name() + BZIP2_EXT;
    command = BZIP2_COMMAND + c->get_archive_dir() + '/' + current_file_name + " " + c->get_files_dir_name();

    cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
    
    delete[] cmd;
    
    return ret;        
}

int zipCompressor::compress(serverSideConfiguration* c)
{
    int ret = 0;
    string command;
    char *cmd;
    
    current_file_name = c->get_archive_name() + ZIP_EXT;
    command = ZIP_COMMAND + c->get_archive_dir() + '/' + current_file_name + " " + c->get_files_dir_name();

    cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
    
    delete[] cmd;
    
    return ret;    
}

int xzCompressor::compress(serverSideConfiguration* c)
{
    int ret = 0;
    string command;
    char *cmd;
    
    current_file_name = c->get_archive_name() + XZ_EXT;
    command = XZ_COMMAND + c->get_archive_dir() + '/' + current_file_name + " " + c->get_files_dir_name();

    cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
    
    delete[] cmd;
    
    return ret;    
}

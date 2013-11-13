/*
 *      configuration.h
 *      Classe rappresentante la configurazione del client
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../common/common.h"

class configuration {
 public:
    configuration();
    explicit configuration(const configuration* orig);
    virtual ~configuration();

    void set_client_ID(int ID);
    int get_client_ID();

    void set_compressor_index(int index);
    int get_compressor_index();
    string get_compressor_name(int index);
    string get_compressor_ext(int index);
    int check_compressor_argument(string compressor);
    bool check_compressors();
    void enable_compressor(int index);
    bool is_compressor_supported(int index);

    void set_archive_name(char *archive_name);
    string get_archive_name();

    void increase_sent_files();
    void set_sent_files_at_compress();
    int get_sent_files();
    int get_sent_files_at_compress();

    bool get_dirty_flag();
    void reset_dirty_flag();
    string get_recv_folder_name(string folder);

 protected:
    char *archive_name;
    int compressor_index;
    int client_ID;
    int sent_files;
    int sent_files_at_compress;
    bool dirty;

    string compressor_name[SUPPORTED_COMPRESSORS];
    string compressor_ext[SUPPORTED_COMPRESSORS];
    bool compressor_enabled[SUPPORTED_COMPRESSORS];
};

#endif  /* CONFIGURATION_H */

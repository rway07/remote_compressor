#ifndef FILEHANDLER_H
#define	FILEHANDLER_H

#include  "common.h"
#include  "exception.h"

class fileHandler 
{
public:
    fileHandler();
    fileHandler(string file_name, int len);
    virtual ~fileHandler();
    
    static bool file_exist(string file);
    static bool is_directory(string directory);
    static bool got_permissions(string directory);
    static string extract_file_name(string path) throw(fileException);
    static int delete_file(string file_name) throw(fileException);
    static int create_directory(string directory) throw(fileException);
    static int delete_directory(string directory) throw(fileException);
    static int get_file_len(string file) throw(fileException);
    static string get_working_dir() throw(fileException);
    static int calculate_MD5(string file_name, unsigned char hash[MD5_DIGEST_LENGTH]) throw(fileException);
    static void print_MD5(unsigned char hash[MD5_DIGEST_LENGTH]);
   
protected:
    
    string current_file_name;
    int len;
};


class fileReader : fileHandler
{
public:
    fileReader(string file_name, int len) throw(fileReaderException);
    ~fileReader();
    int read_trunk(char *buffer, int len);
    void close();

private:
    ifstream file;
};

class fileWriter : fileHandler
{
public:
    fileWriter(string file_name, int len) throw(fileWriterException);
    ~fileWriter();
    int append_trunk(char* trunk, int len);
    void close();
    
private:
    ofstream file;
};
#endif	/* FILEHANDLER_H */


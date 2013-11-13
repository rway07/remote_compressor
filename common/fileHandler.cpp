#include "fileHandler.h"

int fileHandler::calculate_MD5(string file_name, unsigned char hash[MD5_DIGEST_LENGTH]) throw(fileException)
{
    hashwrapper *hasher = new md5wrapper();
    string current_hash;
    
    try
    {
        current_hash = hasher->getHashFromFile(file_name);
    } 
    catch (hlException &e)
    {
        throw fileException();
    }
    
    delete hasher;
    
    strcpy((char*)hash, current_hash.c_str());
    
    return 0;
}

void fileHandler::print_MD5(unsigned char hash[MD5_DIGEST_LENGTH])
{
    /*int j = 0;
    for(j = 0; j < MD5_DIGEST_LENGTH; j++) 
        printf("%02d", hash[j]);*/
    
    cout << hash << endl;
}

bool fileHandler::file_exist(string file) 
{
    int ret = 0;
    int len = file.length();
    char *path = new char[len +1];
    strcpy(path, file.c_str());
    struct stat buf;    //
          
    ret = stat(path, &buf);
     
    delete[] path;
    if (ret == -1)
    {
        return false;
    }
    
    return true;
}

bool fileHandler::is_directory(string directory)
{
    int len = directory.length();
    char *path = new char[len +1];
    strcpy(path, directory.c_str());
    struct stat buf;
    
    stat(path, &buf);
    
    delete[] path;
    
    return S_ISDIR(buf.st_mode);
}

int fileHandler::create_directory(string directory) throw(fileException)
{
    int ret = 0;
    int len = directory.length();
    char *path = new char[len +1];
    strcpy(path, directory.c_str());
    
    ret = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    delete[] path;
    
    if (ret == -1)
    {
        switch (errno)
        {
            case EEXIST:
                //cout << "DEBUG: Directory già esistente." << endl;
                return -1;
                break;
            default:
                throw fileException(errno);
                return -1;
        }        
    }
    
    return 0;
}

int fileHandler::delete_file(string file_name) throw(fileException)
{
    int ret = 0;
    string command = DELETE_COMMAND + file_name;
    char *cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
    delete[] cmd;
    
    if (ret == -1)
    {
        throw fileException(errno);
        return -1;
    }
    
    return 0;
}

int fileHandler::delete_directory(string directory) throw(fileException)
{
    int ret = 0;
    string command = DELETE_COMMAND + directory;
    char *cmd = new char[command.length() +1];
    strcpy(cmd, command.c_str());
    
    ret = system(cmd);
   
    delete[] cmd;
    
    if (ret == -1)
    {
        throw fileException(errno);
        return -1;
    }
       
    return 0;
}

int fileHandler::get_file_len(string file) throw(fileException)
{
    int ret = 0;
    int len = file.length();
    char *path = new char[len +1];
    strcpy(path, file.c_str());
    struct stat buf;    // Idem
    
    ret = stat(path, &buf);
    
    delete[] path;
    if (ret == -1)
    {
        throw fileException(errno);
        return -1;
    }
    
    return buf.st_size;
}

string fileHandler::get_working_dir() throw(fileException)
{
    char *buffer = new char[PATH_MAX +1];
    char *pnt = getcwd(buffer, PATH_MAX);

    if (pnt == NULL)
    {
        throw fileException(errno);
        delete[] buffer;
        return NULL;
    }
    
    string ret = std::string(buffer);
    delete[] buffer;
    
    return ret;
}

bool fileHandler::got_permissions(string directory)
{
    int ret = 0;
    int len = directory.length();
    char *path = new char[len +1];
    strcpy(path, directory.c_str());
    struct stat buf;    // Idem
    
    ret = stat(path, &buf);
    
    delete[] path;
    
    if ((ret == -1) && (errno == EACCES))
    {
        return false;
    }
    else if (ret == -1)
    {
        throw fileException(errno);
        return false;
    }
    
    return true;
}

string fileHandler::extract_file_name(string path) throw(fileException)
{
    int i = 0;
    int pos = 0;
    int len = 0;
    
    if ((path[0] != '/') && (path[0] != '.'))
        return path;
    
    len = path.length();
    for (i = 0; i < len; i++)
        if (path[i] == '/') pos = i;
    
    path.erase(0, pos +1);
    
    return path;
}

fileHandler::fileHandler() {}

fileHandler::fileHandler(string file_name, int len)
{ 
    this->current_file_name = file_name;
    this->len = len;
}

fileHandler::~fileHandler() {}

fileReader::fileReader(string file_name, int len) throw(fileReaderException)
{
    fileHandler(file_name, len);
    this->file.open(file_name.c_str(), ios::in | ios::binary);
    
    if (file.fail()) 
        throw fileReaderException(OPEN);
}

fileReader::~fileReader() {}
fileWriter::~fileWriter() {}

void fileReader::close()
{
    if (this->file.is_open())
    {
        this->file.close();
    }
}

void fileWriter::close()
{
    if (this->file.is_open())
    {
        this->file.close();
    }
}

fileWriter::fileWriter(string file_name, int len) throw(fileWriterException)
{
    fileHandler(file_name, len);
    this->file.open(file_name.c_str(), ios::out | ios::app | ios::binary);
    
    if (file.fail())
        throw fileWriterException(OPEN);
}

int fileReader::read_trunk(char* buffer, int len) 
{
    if (this->file.is_open() == true)
    {
        this->file.read(buffer, len);
    }
    
    return 0;
}

int fileWriter::append_trunk(char* trunk, int len)
{
    if (this->file.is_open() == true)
    {
        this->file.write(trunk, len);
    }
    
    return 0;
}

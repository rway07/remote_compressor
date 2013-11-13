/*
 *      configuration.cpp 
 *      Implementa la classe configuration
 */
#include "configuration.h"

configuration::configuration() 
{
    this->archive_name = new char[MAX_FILENAME_LEN +1];
    strcpy(this->archive_name, DEFAULT_ARCHIVE_NAME);
    this->compressor_index = 0;
    this->sent_files = 0;
    this->sent_files_at_compress = 0;
    this->client_ID = -1; // Fa casino??
    this->dirty = false;
    
    this->compressor_enabled[GNUZIP] = false;
    this->compressor_enabled[BZIP2] = false;
    this->compressor_enabled[ZIP] = false;
    this->compressor_enabled[XZ] = false;
    
    this->compressor_name[GNUZIP] = COMPRESSOR_GNUZIP;
    this->compressor_name[BZIP2] = COMPRESSOR_BZIP2;
    this->compressor_name[ZIP] = COMPRESSOR_ZIP;
    this->compressor_name[XZ] = COMPRESSOR_XZ;
            
    this->compressor_ext[GNUZIP] = GNUZIP_EXT;
    this->compressor_ext[BZIP2] = BZIP2_EXT;
    this->compressor_ext[ZIP] = ZIP_EXT;
    this->compressor_ext[XZ] = XZ_EXT;
}

configuration::configuration(const configuration* orig) 
{
    this->archive_name = new char[MAX_FILENAME_LEN +1];
    strcpy(this->archive_name, orig->archive_name);
    
    this->compressor_index = orig->compressor_index;
    this->sent_files = orig->sent_files;
    this->sent_files_at_compress = orig->sent_files_at_compress;
    this->client_ID = orig->client_ID;
    this->dirty = orig->dirty;
    
    int i = 0;
    for (i=0; i<SUPPORTED_COMPRESSORS; i++)
    {
        this->compressor_enabled[i] = orig->compressor_enabled[i];
        this->compressor_name[i] = orig->compressor_name[i];
        this->compressor_ext[i] = orig->compressor_ext[i];
    }
}

configuration::~configuration() 
{
    if (this->archive_name != 0)
    {
        delete[] this->archive_name;
    }
}

void configuration::enable_compressor(int index)
{
    compressor_enabled[index] = true;
}

bool configuration::is_compressor_supported(int index)
{
    return compressor_enabled[index];
}

string configuration::get_compressor_name(int index)
{
    return compressor_name[index];
}

string configuration::get_compressor_ext(int index)
{
    return compressor_ext[index];
}

int configuration::get_client_ID()
{
    return this->client_ID;
}

void configuration::set_client_ID(int ID)
{
    this->client_ID = ID;
}

void configuration::increase_sent_files()
{
    this->sent_files++;
}

int configuration::get_sent_files()
{
    return this->sent_files;
}

int configuration::get_sent_files_at_compress()
{
    return this->sent_files_at_compress;
}

void configuration::set_sent_files_at_compress()
{
    this->sent_files_at_compress = this->sent_files;
}

void configuration::set_archive_name(char* archive_name)
{
    strcpy(this->archive_name, archive_name);
    this->dirty = true;
}

string configuration::get_archive_name()
{
    string temp(this->archive_name);

    return temp;
}

void configuration::set_compressor_index(int index)
{
    this->compressor_index = index;
    this->dirty = true;
}

int configuration::get_compressor_index()
{
    return this->compressor_index;
}


bool configuration::check_compressors()
{
    int ret = 0;
    char *path = new char[sizeof(TAR_PATH) +1];
    strcpy(path, TAR_PATH);
    struct stat buf;
    
    ret = stat(path, &buf);
    
    if (ret == -1) 
    {
        cout << "ERRORE: tar è necessario per il funzionamento del programma! Uscita forzata!!" << endl;
        return false;
    }
    
    delete[] path;
    
    compressor_enabled[GNUZIP] = true;
    compressor_enabled[BZIP2] = true;
    
    // Check for zip
    path = new char[sizeof(ZIP_PATH) +1];
    strcpy(path, ZIP_PATH);
    
    ret = stat(path, &buf);
    
    delete[] path;
    
    if (ret == -1)
    {
        cout << "WARNING: zip non disponibile." << endl;
        compressor_enabled[ZIP] = false;
    } 
    else 
        compressor_enabled[ZIP] = true;
    
    // Check for xz
    path = new char[sizeof(XZ_CHECK_COMMAND) +1];
    strcpy(path, XZ_CHECK_COMMAND);
    
    ret = system(path);
    
    delete[] path;
    
    if (ret!= 0)
    {
        cout << "WARNING: xz non disponibile." << endl;
        compressor_enabled[XZ] = false;
    } 
    else
        compressor_enabled[XZ] = true;
    
    return true; 
}

// Questa funzione controlla se l'applicazione supporta il compressore,
// Però e poi il server a dare conferma con il comando configure-compressor
int configuration::check_compressor_argument(string compressor)
{
    if (compressor.compare(COMPRESSOR_GNUZIP) == 0)
    {
        return GNUZIP;
    }
    else if (compressor.compare(COMPRESSOR_BZIP2) == 0)
    {
        return BZIP2;
    }
    else if ((compressor.compare(COMPRESSOR_ZIP) == 0) && (compressor_enabled[ZIP] == true))
    {
        return ZIP;
    }
    else if ((compressor.compare(COMPRESSOR_XZ) == 0) && (compressor_enabled[XZ] == true))
    {
        return XZ;
    }
    
    return -1;
}

string configuration::get_recv_folder_name(string folder)
{
    stringstream ss;
    ss << client_ID;
            
    folder = folder + "/ricevuti_client" + ss.str();
    
    return folder;
}

bool configuration::get_dirty_flag()
{
    return this->dirty;
}

void configuration::reset_dirty_flag()
{
    this->dirty = false;
}

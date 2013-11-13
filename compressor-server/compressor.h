#ifndef COMPRESSOR_H
#define	COMPRESSOR_H

#include "../common/serverSideConfiguration.h"

class compressor
{
public:
    compressor();
    virtual ~compressor();
    //int compress(serverSideConfiguration *c);
    virtual int compress(serverSideConfiguration *c) = 0;
    string get_archive_name();

protected:   
    string current_file_name;
};

class compressorBridge
{
public:
    compressorBridge();
    void set_compression_algorithm(compressor *alg);
    int compress(serverSideConfiguration *c);
    string get_archive_name();
    
private:
    compressor *algorithm;
};

class gnuzipCompressor : public compressor
{
public:
    ~gnuzipCompressor();
    int compress(serverSideConfiguration *c);
};

class bzip2Compressor : public compressor
{
public:
    ~bzip2Compressor();
    int compress(serverSideConfiguration *c);
};

class zipCompressor : public compressor
{
public:
    ~zipCompressor();
    int compress(serverSideConfiguration *c);
};


class xzCompressor : public compressor
{
public:
    ~xzCompressor();
    int compress(serverSideConfiguration *c);
};
#endif	/* COMPRESSOR_H */


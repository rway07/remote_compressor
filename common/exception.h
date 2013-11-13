/*
 *      exception.h
 *      Definisce le varie eccezioni che il client/server possono sollevare
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "common.h"

enum transmissionExceptionSource {
    SEND_COMMAND,
    SEND_RESPONSE,
    SEND_DATA,
    RECEIVE_COMMAND,
    RECEIVE_RESPONSE,
    RECEIVE_DATA
};

enum serverErrorType {
    SOCKET = 1,
    BIND,
    LISTEN,
    ACCEPT,
    WORKINGDIR
};

enum commandExceptionType {
    UNKNOWN_COMMAND,
    BAD_COMMAND,
    INCOMPLETE_COMMAND,
    OUT_OF_LEN_COMMAND
};

enum commandExecutionExceptionType {
    COMMAND_ERROR,
    ALREADY_EXECUTED_COMMAND,
    ALREADY_COMPRESSED_ARCHIVE,
    NO_SENT_FILES,
};

// networkException

class networkException {
public:
    virtual void show_error() = 0;
};

class serverErrorException : networkException {
public:
    serverErrorException(serverErrorType type);
    void show_error();
private:
    serverErrorType source;
    void show_listen_error();
};

class transmissionException : networkException {
public:
    transmissionException(transmissionExceptionSource source);
    void show_error();

protected:
    transmissionExceptionSource source;
};

class notConnectedException : networkException {
public:
    void show_error();
};

class badPortException : networkException {
public:
    void show_error();
};

class clientConnectException : networkException {
public:
    void show_error();
};

class badAddressException : networkException {
public:
    void show_error();
};

// commandExecutionException

class commandExecutionException {
public:
    commandExecutionException(commandExecutionExceptionType type);

    void show_error();

private:
    commandExecutionExceptionType type;
};

// paramException

class paramException {
public:
    virtual void show_error() = 0;
};

class badPortParamException : paramException {
public:
    void show_error();
};

class badAddressParamException : paramException {
public:
    void show_error();
};

class badParamsNumberException : paramException {
public:
    void show_error();
};

class noParamsClientException : paramException {
public:
    void show_error();
};

class noParamsServerException : paramException {
public:
    void show_error();
};

// commandException

class commandException {
public:
    commandException(commandExceptionType type);
    void show_error();

private:
    commandExceptionType error_type;
};

//argumentException

class argumentException {
public:
    virtual void show_error() = 0;
};

class badFileNameArgumentException : argumentException {
public:
    void show_error();
};

class badCompressorArgumentException : argumentException {
public:
    void show_error();
};

class badDirectoryArgumentException : argumentException {
public:
    void show_error();
};

// configurationException

class configurationException {
public:
    virtual void show_error() = 0;
};

class nameConfigurationException : configurationException {
public:
    void show_error();
};

class compressorConfigurationException : configurationException {
public:
    void show_error();
};

class configurationIntegrityCheckException : configurationException {
public:
    void show_error();
};

class wrongMD5Exception {
public:
    void show_error();
};

class sentFileAlreadyExistException {
public:
    void show_error();
};

//----------------------------------------------------
//----------------------------------------------------

enum fileExceptionSource {
    OPEN,
    READ,
    WRITE,
    CLOSE
};

class fileException {
public:
    fileException();
    fileException(int error);
    void show_error();

protected:
    int error;
};

class fileReaderException : fileException {
public:
    fileReaderException(fileExceptionSource source);
    void show_error();

private:
    fileExceptionSource source;
};

class fileWriterException : fileException {
 public:
    fileWriterException(fileExceptionSource source);
    void show_error();

 private:
    fileExceptionSource source;
};

#endif  /* EXCEPTION_H */

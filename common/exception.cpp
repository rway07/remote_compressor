/*
 *      exception.cpp
 *      Implementazione delle classi rappresentanti le eccezioni
 */

#include "exception.h"

void notConnectedException::show_error() {
    cout << "ATTENZIONE: La connessione è stata chiusa!! Uscita..." << endl;
}

void badPortException::show_error() {
    cout << "ERRORE: Parametro [port] non valido!!" << endl;
}

transmissionException::transmissionException(transmissionExceptionSource source) {
    this->source = source;
}

void transmissionException::show_error() {
    switch (this->source) {
        case SEND_COMMAND:
            cout << "ERRORE: Errore nell'invio del comando!!" << endl;
            break;
        case SEND_RESPONSE:
            cout << "ERRORE: Errore nell'invio della risposta!!" << endl;
            break;
        case SEND_DATA:
            cout << "ERRORE: Errore nell'invio dei dati!!" << endl;
            break;
        case RECEIVE_COMMAND:
            cout << "ERRORE: Errore nella ricezione del comando!!" << endl;
            break;
        case RECEIVE_RESPONSE:
            cout << "ERRORE: Errore nella ricezione della risposta!!" << endl;
            break;
        case RECEIVE_DATA:
            cout << "ERRORE: Errore nella ricezione dei dati!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore sconosiuto nella trasmissione dei dati!!" << endl;
            break;
    }
}

commandExecutionException::commandExecutionException(commandExecutionExceptionType type) {
    this->type = type;
}

void commandExecutionException::show_error() {
    switch (this->type) {
        case ALREADY_EXECUTED_COMMAND:
            cout << "NOTIFY: Comando già eseguito e non necessario." << endl;
            break;
        case NO_SENT_FILES:
            cout << "ERRORE: Impossibile eseguire comand compress!! Nessun file inviato al server!!" << endl;
            break;
        case ALREADY_COMPRESSED_ARCHIVE:
            cout << "ERRORE: Archivio già ricevuto e nessuna modifica alla configurazione!!" << endl;
            break;
        case COMMAND_ERROR:
            cout << "ERRORE: Errore nell'esecuzione del comando!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore nell'esecuzione del comando!!" << endl;
            break;
    }
}

void badPortParamException::show_error() {
    cout << "ERRORE: Parametro [port] non valido!!" << endl;
}

void badAddressParamException::show_error() {
    cout << "ERRORE: Parametro [host] non valido!!" << endl;
}

void noParamsServerException::show_error() {
    cout << "USAGE: compressor-server [port]" << endl;
}

void noParamsClientException::show_error() {
    cout << "USAGE: compressor-client [host] [port]" << endl;
}

void badParamsNumberException::show_error() {
    cout << "ERRORE: Errore nei parametri passati al server!!" << endl;
}

serverErrorException::serverErrorException(serverErrorType type) {
    this->source = type;
}

void serverErrorException::show_listen_error() {
    switch (errno) {
        case EADDRINUSE:
            cout << "ERRORE: Un altro socket è in ascolto sulla stessa porta!!" << endl;
            break;
        case ECONNREFUSED:
            cout << "ERRORE: Connessione rifiutata. Raggiunto limite connessioni!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore nella listen()!!" << endl;
            break;
    }
}

void serverErrorException::show_error() {
    switch (this->source) {
        case SOCKET:
            cout << "ERRORE: Errore nella creazione del socket!!" << endl;
            break;
        case BIND:
            cout << "ERRORE: Errore nella bind()!!" << endl;
            break;
        case LISTEN:
            show_listen_error();
            break;
        case ACCEPT:
            cout << "ERRORE: Errore nella accept()!!" << endl;
            break;
        case WORKINGDIR:
            cout << "ERRORE: Errore nella directory di lavoro!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore sconosciuto nel server!!" << endl;
            break;
    }
}

commandException::commandException(commandExceptionType type) {
    this->error_type = type;
}

void commandException::show_error() {
    switch (this->error_type) {
        case UNKNOWN_COMMAND:
            cout << "ERRORE: Comando sconosciuto!!" << endl;
            break;
        case BAD_COMMAND:
            cout << "ERRORE: Comando non valido!!" << endl;
            break;
        case INCOMPLETE_COMMAND:
            cout << "ERRORE: Comando incompleto!!" << endl;
            break;
        case OUT_OF_LEN_COMMAND:
            cout << "ERRORE: Lunghezza del comando oltre i limiti consentiti!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore sconosciuto nel comando!!" << endl;
            break;
    }
}

void badCompressorArgumentException::show_error() {
    cout << "ERRORE: Compressore non supportato!!" << endl;
}

void badFileNameArgumentException::show_error() {
    cout << "ERRORE: Nome del file invalido!!" << endl;
}

void badDirectoryArgumentException::show_error() {
    cout << "ERRORE: La directory specificata non è valida!!" << endl;
}

void configurationIntegrityCheckException::show_error() {
    cout << "ERRORE: Errore nel controllo della configurazione!!" << endl;
}

void nameConfigurationException::show_error() {
    cout << "ERRORE: Errore nella configurazione nel nome dell'archivio!!" << endl;
}

void compressorConfigurationException::show_error() {
    cout << "ERRORE: Errore nella configurazione del compressore!!" << endl;
}

void clientConnectException::show_error() {
    cout << "ERRORE: Errore nella connessione al server!!" << endl;
}

void badAddressException::show_error() {
    cout << "ERRORE: Indirizzo del server non valido!!" << endl;
}

fileException::fileException() {
}

fileException::fileException(int error) {
    this->error = error;
    // cout << "DEBUG: errno " << this->error << endl;
}

void fileException::show_error() {
    switch (this->error) {
        case EACCES:
            cout << "ERRORE: Permesso negato!!" << endl;
            break;
        case ENOENT:
            cout << "ERRORE: Il file non esiste!!" << endl;
            break;
        case ENOTDIR:
            cout << "ERRORE: Una componente del path non corrisponde ad una directory!!" << endl;
            break;
        default:
            cout << "ERRORE: Errore nell'accesso al file!!" << endl;
            break;
    }

    errno = 0;
}

fileReaderException::fileReaderException(fileExceptionSource source) : fileException() {
    this->source = source;
}

void fileReaderException::show_error() {
    switch (this->source) {
        case OPEN:
            cout << "ERRORE: FileReader() Errore nell'apertura del file" << endl;
            break;
        case READ:
            cout << "ERRORE: FileReader() Errore nella lettura del file" << endl;
            break;
        case WRITE:
            break;
        case CLOSE:
            cout << "ERRORE: FileReader() Errore nella chiusura del file" << endl;
            break;
    }
}

fileWriterException::fileWriterException(fileExceptionSource source) : fileException() {
    this->source = source;
}

void fileWriterException::show_error() {
    switch (this->source) {
        case OPEN:
            cout << "ERRORE: FileWriter() Errore nell'apertura del file" << endl;
            break;
        case READ:
            break;
        case WRITE:
            cout << "ERRORE: FileWriter() Errore nella scrittura del file" << endl;
            break;
        case CLOSE:
            cout << "ERRORE: FileWriter() Errore nella chiusura del file" << endl;
            break;
    }
}

void wrongMD5Exception::show_error() {
    cout << "ERRORE: Checksum MD5 del file non corretto!!" << endl;
}

void sentFileAlreadyExistException::show_error() {
    cout << "SERVER: Il file è già stato inviato ed è salvato integro su disco!" << endl;
}

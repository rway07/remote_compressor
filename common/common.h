#ifndef COMMON_H
#define	COMMON_H

//#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <exception>
//#include <limits.h>
#include <linux/limits.h>
#include "../common/hashlib2plus/hashlibpp.h"
#include <pthread.h>

#define SA struct sockaddr

#define MD5_DIGEST_LENGTH 32

#define CLIENT_PARAM_HOST_POS 1
#define CLIENT_PARAM_PORT_POS 2
#define SERVER_PARAM_PORT_POS 1

#define MIN_PORT 1024
#define MAX_PORT 65535

#define CMD_POS 0
#define PARAM_POS 1

#define ADDRESS_LEN 8
#define MAX_FILENAME_LEN 32
#define MAX_CMD_LEN 128
#define MAX_CMD_TOKENS 2
#define CMD_REAL_LEN 5
#define RESPONSE_REAL_LEN 4
#define NET_CMD_LEN 4
#define NET_RESPONSE_LEN 3

#define NET_EXIT_CMD "cmd0"
#define NET_HELP_CMD "cmd1"
#define NET_SEND_CMD "cmd2"
#define NET_COMPRESS_CMD "cmd3"
#define NET_SHOW_CFG_CMD "cmd4"
#define NET_CFG_NAME_CMD "cmd5"
#define NET_CFG_COMP_CMD "cmd6"
#define NET_CONF_CHECK_CMD "cmd9"

#define EXIT_CMD "quit"
#define HELP_CMD "help"
#define SEND_CMD "send"
#define COMPRESS_CMD "compress"
#define SHOW_CFG_CMD "show-configuration"
#define CFG_NAME_CMD "configure-name"
#define CFG_COMP_CMD "configure-compressor"

#define DEFAULT_ARCHIVE_NAME "archivio"
#define SUPPORTED_COMPRESSORS 4
#define COMPRESSOR_GNUZIP "gnuzip"
#define COMPRESSOR_BZIP2 "bzip2"
#define COMPRESSOR_ZIP "zip"
#define COMPRESSOR_XZ "xz"
#define BZIP2_EXT ".tar.bz2"
#define GNUZIP_EXT ".tar.gz"
#define XZ_EXT ".tar.xz"
#define ZIP_EXT ".zip"
#define ZIP_COMMAND "zip -q -r "
#define XZ_COMMAND "tar cJf "
#define BZIP2_COMMAND "tar cjf "
#define GNUZIP_COMMAND "tar czf "
#define XZ_CHECK_COMMAND "tar --help | grep xz -q"
#define TAR_PATH "/bin/tar"
#define ZIP_PATH "/usr/bin/zip"
#define DELETE_COMMAND "rm -rf "

#define EXIT_COMMAND_NOTIFY -5
#define NOT_CONNECTED_NOTIFY -6

#define NET_RESPONSE_OK "ack"
#define NET_RESPONSE_BYE "bye"
#define NET_RESPONSE_ERROR "err"
#define NET_RESPONSE_CONTINUE "cnt"

#define MAX_PENDING_CONNECTIONS 10
#define MAX_CLIENT 16
#define NET_TRUNK 65535

using namespace std;

enum COMPRESSOR
{
    GNUZIP,
    BZIP2,
    ZIP,
    XZ
};

#endif	/* COMMON_H */

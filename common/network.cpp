#include "network.h"

network::network() {}

network::network(const network& orig) {}

network::~network() {}

int network::send_int(int socket, int num) throw(transmissionException)
{
    int ret = 0;
    int len = num;
    
    ret = send(socket, (void*)&len, sizeof(len), MSG_NOSIGNAL);
        
    if (ret == -1)
    {
        throw transmissionException(SEND_DATA);
        return -1;
    }
    
    return 0;
}

int network::receive_int(int socket) throw(transmissionException, notConnectedException)
{
    int ret = 0;
    int len;
    
    ret = recv(socket, (void*)&len, sizeof(len), MSG_WAITALL);

    if (ret == -1)
    {
        throw transmissionException(RECEIVE_DATA);
        return -1;
    }
    
    return len;
}

int network::send_command(int socket, char command[CMD_REAL_LEN]) throw(transmissionException)
{
    int ret = 0;
    
    ret = send(socket, (void*)command, NET_CMD_LEN, MSG_NOSIGNAL);
    
    if (ret == -1)
    {
        cout << "DEBUG: errno=" << errno << endl;
        throw transmissionException(SEND_COMMAND);
        return -1;
    }
    
    return 0;
}

int network::receive_command(int socket, char command[CMD_REAL_LEN]) 
        throw(transmissionException, notConnectedException)
{
    int ret = 0;
    
    ret = recv(socket, (void*)command, NET_CMD_LEN, MSG_WAITALL);
    command[NET_CMD_LEN] = '\0';
                                
    switch (ret)
    {
        case 0:
            throw notConnectedException();
            return -1;
            break;
        case -1:
            throw transmissionException(RECEIVE_COMMAND);
            return -1;
            break;
        default:
            //if (errno == ENOTCONN) cout << "DEBUG: NON CONNESSO : " << errno << endl;
            break;
    }
    
    return 0;
}

int network::send_response(int socket, networkResponse response) throw(transmissionException)
{
    int ret = 0;
    char rsp[RESPONSE_REAL_LEN];
    
    switch (response)
    {
        case ERROR:
            strcpy(rsp, NET_RESPONSE_ERROR);
            break;
        case OK:
            strcpy(rsp, NET_RESPONSE_OK);
            break;
        case BYE:
            strcpy(rsp, NET_RESPONSE_BYE);
            break;
        case CONTINUE:
            strcpy(rsp, NET_RESPONSE_CONTINUE);
            break;
        default:
            strcpy(rsp, "wtf");
            break;
    }
        
    ret = send(socket, (void*)rsp, NET_RESPONSE_LEN, MSG_NOSIGNAL);
                
    if (ret == -1)
    {
        throw transmissionException(SEND_RESPONSE);
        return -1;
    }
    
    return 0;    
}

int network::receive_response(int socket) 
        throw(transmissionException, commandExecutionException, notConnectedException)
{
    char response[RESPONSE_REAL_LEN];
    int ret = 0;
    
    strcpy(response, "");    
    ret = recv(socket, (void*)response, NET_RESPONSE_LEN, MSG_WAITALL);
    response[NET_RESPONSE_LEN] = '\0';
      
    if (ret == -1) 
    {
        throw transmissionException(RECEIVE_RESPONSE);
        return -1;
    }
    else if (ret == 0)
    {
        throw notConnectedException();
        return -1;
    }
    else if (strcmp(response, NET_RESPONSE_ERROR) == 0)
    {
        throw commandExecutionException(COMMAND_ERROR);
        return -1;
    }
    else if (strcmp(response, NET_RESPONSE_CONTINUE) == 0)
    {
        throw commandExecutionException(ALREADY_EXECUTED_COMMAND);
        return 0;
    }
    
    return 0;    
}

int network::send_data(int socket, char* buffer, int len) throw(transmissionException)
{
    int ret = 0;
    
    ret = send(socket, (void*)buffer, len, MSG_NOSIGNAL);
    
    if (ret == -1) 
    {
        throw transmissionException(SEND_DATA);
        return -1;
    }
    
    return 0;    
}

int network::receive_data(int socket, char* buffer, int len) 
        throw(transmissionException, notConnectedException)
{
    int ret = 0;
    
    ret = recv(socket, (void*)buffer, len, MSG_WAITALL);
    buffer[len] = '\0';
        
    if (ret == -1) 
    {
        throw transmissionException(RECEIVE_DATA);
        return -1;
    }
    
    return 0;    
}

int network::receive_file(int socket, char* buffer, int len) throw(transmissionException, notConnectedException)
{
    int ret = 0;
    
    ret = recv(socket, (void*)buffer, len, MSG_WAITALL);
        
    if (ret == -1) 
    {
        throw transmissionException(RECEIVE_DATA);
        return -1;
    }
    
    return 0;    
}

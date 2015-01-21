#include "net_c.h"

#include <strings.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

int get_public_ip_address(char *buffer, int buffersize)
{
    const char *google_dns_server = "8.8.8.8";
    int dns_port = 53;
    
    struct sockaddr_in serv;
    
    int sock = socket ( AF_INET, SOCK_DGRAM, 0);
    
    //Socket could not be created
    if (sock < 0)
    {
        printf("Socket error");
        return IP_ADDRESS_SOCKET_ERROR;
    }
    
    memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );
    
    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );
    
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);

    const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, buffersize);
    
    if (p != NULL)
    {
        printf("Local ip is : %s \n" , buffer);
    }
    else
    {
        //Some error
        printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
        return errno;
    }
    
    close(sock);
    
    return IP_ADDRESS_SUCCESS;
}
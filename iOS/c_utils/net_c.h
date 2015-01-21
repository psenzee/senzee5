#ifndef _NET_C_H
#define _NET_C_H

#ifdef __cplusplus
extern "C" {
#endif

enum { IP_ADDRESS_SUCCESS = 0, IP_ADDRESS_SOCKET_ERROR = -1, };
    
int get_public_ip_address(char *buffer, int buffersize);
    
#ifdef __cplusplus
}
#endif

#endif // _NET_C_H
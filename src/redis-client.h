#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <evcom.h>

typedef struct {
    evcom_stream stream;    
    char *host;
    short port;
} redis_client_t;

redis_client_t*
redis_client_create(char *host, short port);

#endif


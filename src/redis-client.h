#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <evcom.h>

typedef struct {
    int err;
} redis_error_t;

struct redis_client_t;
struct redis_client_t {
    char *host;
    short port;
    evcom_stream stream;    

    /* Public */
    int (*on_response)(struct redis_client_t *c);
    int (*on_error)(struct redis_client_t *c, redis_error_t *err);
};

typedef struct redis_client_t redis_client_t;

redis_client_t*
redis_client_create(char *host, short port);

#endif


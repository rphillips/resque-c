#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <ev.h>
#include <evcom.h>

typedef struct {
    int err;
} redis_error_t;

struct redis_client_t;
struct redis_client_t {
    /* libev datatypes */
    evcom_stream stream;    

    /* Public Variables */
    char *host;
    short port;

    /* Public Callbacks */
    int (*on_response)(struct redis_client_t *c);
    int (*on_error)(struct redis_client_t *c, redis_error_t *err);

    /* Private Variables */
    struct sockaddr_in tcp_address;
};

typedef struct redis_client_t redis_client_t;

/** 
 * @brief 
 * 
 * @param host
 * @param port
 */
redis_client_t*
redis_client_create(char *host, short port, EV_P );

void
redis_client_destroy(redis_client_t *cli);

void
redis_client_get(redis_client_t *cli, const char *key);

#endif


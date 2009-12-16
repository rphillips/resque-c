#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <ev.h>
#include <evcom.h>

typedef struct redis_error_t {
    int err;
} redis_error_t;

struct redis_client_t {
    /* libev datatypes */
    evcom_stream stream;    

    /* Public Variables */
    char *host;
    short port;
    int verbose;

    /* Public Callbacks */
    int (*on_response)(struct redis_client_t *c);
    int (*on_error)(struct redis_client_t *c, redis_error_t *err);

    /* Private Variables */
    struct sockaddr_in tcp_address;
    evcom_queue deferred_queue;
};

typedef struct redis_client_t redis_client_t;

/* Deferred Support */
struct redis_deferred_t;
typedef int(*redis_deferred_cb)(struct redis_deferred_t *d, 
                                char *buffer, 
                                size_t len,
                                void *baton);

typedef struct redis_deferred_t {
    redis_client_t *cli;
    redis_deferred_cb cb;
    evcom_queue queue;
    void *baton;
} redis_deferred_t;

/** 
 * @brief 
 * @param host
 * @param port
 */
redis_client_t*
redis_client_create(char *host, short port, EV_P );

redis_deferred_t*
redis_deferred_create(redis_client_t *cli, redis_deferred_cb cb, void *baton);

void
redis_client_set_verbosity(redis_client_t *cli, int verbosity);

void
redis_client_destroy(redis_client_t *cli);

void
redis_client_get(redis_client_t *cli, const char *key, redis_deferred_t *cdefer);

void
redis_client_lpop(redis_client_t *cli, const char *key, redis_deferred_t *cdefer);

#endif


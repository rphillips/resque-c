/*
Copyright (c) 2009 Ryan Phillips <ryan@trolocsis.com>
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
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


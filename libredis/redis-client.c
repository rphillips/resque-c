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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "redis-client.h"

#define REDIS_DEFAULT_PORT    (6379)
#define REDIS_TIMEOUT_CONNECT (10.0)

redis_deferred_t*
redis_deferred_create(redis_client_t *cli, redis_deferred_cb cb, void *baton)
{
    redis_deferred_t *d = malloc(sizeof(*d));
    d->cb = cb;
    d->cli = cli;
    d->baton = baton;
    return d;
}

static void
redis_deferred_free(redis_deferred_t *d)
{
    free(d);
}

static int
redis_cb_get(redis_deferred_t *d, char *buffer, size_t len)
{
    return 0;
}

static void 
redis_on_connect(evcom_stream *stream)
{
}

static void 
redis_on_timeout(evcom_stream *stream)
{
}

static int 
redis_client_sendf(redis_client_t *cli, const char *fmt, ...)
{
    char buffer[4096];
    int length;
    va_list args;

    va_start(args, fmt);
    length = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    evcom_stream_write(&cli->stream, buffer, length);

    return 0;
}

static void
redis_on_read(evcom_stream *stream, const void *buf, size_t len)
{
    redis_client_t *cli = stream->data;

    if (cli->verbose > 1) {
        int i;
        for (i=0; i<len; i++) {
            fprintf(stderr, "%c", ((const char*)buf)[i]);
        }
        fprintf(stderr, "\n");
    }

    while (!evcom_queue_empty(&cli->deferred_queue)) {
        evcom_queue *q = evcom_queue_last(&cli->deferred_queue);
        redis_deferred_t *d = evcom_queue_data(q, redis_deferred_t, queue);
        d->cb(d, buf, len, d->baton);
        evcom_queue_remove(q);
        redis_deferred_free(d);
    }
}

static void 
redis_on_close(struct evcom_stream *stream)
{
    redis_client_t *cli = stream->data;
    if (stream->flags & EVCOM_CONNECTED) {
        printf("on close (connected socket)\n");
    }
    else {
        printf("on close (error on connect %s:%d)\n", cli->host, cli->port);
    }
    redis_client_destroy(cli);
}

void
redis_client_destroy(redis_client_t *cli)
{
    free(cli->host);     
    free(cli);     
}

void
redis_client_set_verbosity(redis_client_t *cli, int verbosity)
{
    cli->verbose = verbosity;
}

redis_client_t*
redis_client_create(char *host, short port, EV_P)
{
    redis_client_t *cli;
    int rc;
    
    cli = malloc(sizeof(redis_client_t));
    cli->host = host ? strdup(host) : strdup("127.0.0.1");
    cli->port = port ? port : REDIS_DEFAULT_PORT;
    cli->verbose = 0;

    /* setup evcom stream */
    evcom_stream_init(&cli->stream);
    cli->stream.on_connect = redis_on_connect;
    cli->stream.on_read    = redis_on_read;
    cli->stream.on_timeout = redis_on_timeout;
    cli->stream.on_close   = redis_on_close;
    cli->stream.data       = cli;

    /* setup deferred queue */
    evcom_queue_init(&cli->deferred_queue);

    /* setup IP address */
    memset(&cli->tcp_address, 0, sizeof(struct sockaddr_in));
    cli->tcp_address.sin_family = AF_INET;
    cli->tcp_address.sin_port = htons(cli->port);
    inet_aton(cli->host, &cli->tcp_address.sin_addr.s_addr);

    rc = evcom_stream_connect(&cli->stream, 
                             (struct sockaddr*)&cli->tcp_address);
    if (rc != 0) {
        fprintf(stderr, "Error connecting to redis (%s:%p)\n", host, port);
        redis_client_destroy(cli);
        return NULL;
    }

    /* Attach the stream to the event loop */
    evcom_stream_attach(EV_DEFAULT_ &cli->stream);

    return cli;
}

void
redis_client_get(redis_client_t *cli, const char *key, redis_deferred_t *cdefer)
{
    redis_deferred_t *d = redis_deferred_create(cli, redis_cb_get, NULL);
    redis_client_sendf(cli, "GET %s\r\n", key);
    evcom_queue_insert_head(&cli->deferred_queue, &d->queue);
    if (cdefer) {
        evcom_queue_insert_head(&cli->deferred_queue, &cdefer->queue);
    }
}

void
redis_client_lpop(redis_client_t *cli, const char *key, redis_deferred_t *cdefer)
{
    redis_deferred_t *d = redis_deferred_create(cli, redis_cb_get, NULL);
    redis_client_sendf(cli, "LPOP %s\r\n", key);
    evcom_queue_insert_head(&cli->deferred_queue, &d->queue);
    if (cdefer) {
        evcom_queue_insert_head(&cli->deferred_queue, &cdefer->queue);
    }
}

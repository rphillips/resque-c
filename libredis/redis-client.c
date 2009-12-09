#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "redis-client.h"

#define REDIS_DEFAULT_PORT    (6379)
#define REDIS_TIMEOUT_CONNECT (10.0)

static void redis_on_connect(evcom_stream *stream)
{
    printf("on connect\n");
}

static void redis_on_timeout(evcom_stream *stream)
{
    printf("on timeout\n");
}

static void
redis_on_read(evcom_stream *stream, const void *buf, size_t len)
{
    int i;
    printf("on read\n");

    for (i=0; i<len; i++) {
        printf("%c", ((const char*)buf)[i]);
    }
}

static void redis_on_close(struct evcom_stream *stream)
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

redis_client_t*
redis_client_create(char *host, short port, EV_P)
{
    redis_client_t *cli;
    int rc;
    
    cli = malloc(sizeof(redis_client_t));
    cli->host = host ? strdup(host) : strdup("127.0.0.1");
    cli->port = port ? port : REDIS_DEFAULT_PORT;

    /* setup evcom stream */
    evcom_stream_init(&cli->stream);
    cli->stream.on_connect = redis_on_connect;
    cli->stream.on_read    = redis_on_read;
    cli->stream.on_timeout = redis_on_timeout;
    cli->stream.on_close   = redis_on_close;
    cli->stream.data       = cli;
    evcom_stream_reset_timeout(&cli->stream, REDIS_TIMEOUT_CONNECT);

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

void
redis_client_get(redis_client_t *cli, const char *key)
{
    redis_client_sendf(cli, "GET %s\r\n", key);
}


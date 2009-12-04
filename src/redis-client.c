#include <stdlib.h>
#include <string.h>
#include "redis-client.h"

#define REDIS_DEFAULT_PORT (6379)

redis_client_t*
redis_client_create(char *host, short port)
{
    redis_client_t *cli = malloc(sizeof(redis_client_t));
    if (!cli) {
        return NULL;
    }
    cli->host = host ? host : strdup("127.0.0.1");
    cli->port = port ? port : REDIS_DEFAULT_PORT;
    evcom_stream_init(&cli->stream);
    return cli;
}

#include <stdio.h>
#include <ev.h>
#include "redis-client.h"

int cb_get(redis_deferred_t *d, char *buffer, size_t len)
{
    redis_client_lpop(d->cli, "blah", NULL);
    return 0;
}

int main()
{
    redis_client_t *client;
    struct ev_loop *loop;
    
    loop = ev_default_loop(0);
    
    client = redis_client_create(NULL, 0, loop);
    redis_client_set_verbosity(client, 2);

    redis_client_get(client, "test", 
                     redis_deferred_create(client, cb_get));

    ev_loop(loop, 0);

    return 0;
}


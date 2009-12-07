#include <ev.h>
#include "redis-client.h"

int main()
{
    redis_client_t *client;
    struct ev_loop *loop;
    
    loop = ev_default_loop(0);
    
    client = redis_client_create(NULL, 0, loop);

    ev_loop(loop, 0);

    return 0;
}

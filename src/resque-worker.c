#include <stdio.h>
#include "resque.h"
#include "redis-client.h"

static void
timer_cb(EV_P_ ev_timer *w, int revents)
{
    fprintf(stderr, "Timer CB\n");
}

resque_error_t*
resque_worker(resque_t *resque)
{
    struct ev_loop *loop = ev_default_loop(0);

    ev_timer_init(&resque->timer, timer_cb, 0., 1);
    ev_timer_start(loop, &resque->timer);
    
#if 0
    client = redis_client_create(NULL, 0, loop);
    redis_client_set_verbosity(client, 2);
    redis_client_get(client, "test", 
                     redis_deferred_create(client, cb_get));
#endif

    ev_loop(loop, 0);

    return RESQUE_SUCCESS;
}


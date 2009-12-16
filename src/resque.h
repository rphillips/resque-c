#ifndef RESQUE_H_
#define RESQUE_H_

#include <ev.h>
#include "redis-client.h"
#include "resque-error.h"

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

typedef struct resque_t {
    int worker;      /* Are we are worker? */

    /* scheduler */
    ev_timer timer;  /* check for new jobs every... */
    float interval;  /* seconds */

    /* redis */
    redis_client_t *redis; /* client */

    /* job */
    ev_child job;    /* the job watcher */
    int      job_running;
} resque_t;

resque_error_t* 
resque_worker(resque_t *resque);

#endif


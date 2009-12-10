#ifndef RESQUE_H_
#define RESQUE_H_

#include <ev.h>
#include "resque-error.h"

typedef struct resque_t {
    int worker;      /* Are we are worker? */
    ev_timer timer;  /* check for new jobs every... */
    float interval;  /* seconds */
} resque_t;

resque_error_t* 
resque_worker(resque_t *resque);

#endif


#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <stdlib.h>
#include "resque.h"
#include "redis-client.h"

static void
child_cb (EV_P_ ev_child *w, int revents)
{
    resque_t *resque = w->data;
    ev_child_stop (EV_DEFAULT_ &resque->job);
    resque->job_running = FALSE ;
    fprintf(stderr, "process %d exited with status %x\n", w->rpid, w->rstatus);
}

static int
execute_job(resque_t *resque, json_t *json)
{
    pid_t pid = fork ();
    if (pid < 0) {
    }
    else if (pid == 0) {
        exit(1);
    }
    else {
        resque->job_running = TRUE;
        resque->job.data = resque;
        ev_child_init(&resque->job, child_cb, pid, 0);
        ev_child_start(EV_DEFAULT_ &resque->job);
    }
    return 0;
}

static int 
lpop_cb(redis_deferred_t *d, char *buffer, size_t len, void *baton)
{
    resque_t *resque = baton;

    /* Null terminate buffer */
    buffer[len] = 0;

    /* Do nothing for an empty redis response */
    if (strncmp(buffer, "$-1", 3) == 0) {
        return 0;
    }

    /* Response */
    if (buffer[0] == '$') {
        char *p;
        json_t *json;
        json_error_t json_err;

        if ((p=strstr(buffer, "\r\n")) == NULL) {
            return -1;
        }

        p += 2;
        buffer[len - 2] = '\0';

        /* Load payload into json buffer */
        if ((json=json_loads(p, &json_err)) == NULL) {
            fprintf(stderr, "invalid json buffer (%s:%i)\n", json_err.text, json_err.line);
            return -1;
        }

        char *str = json_dumps(json, 0);
        if (str) {
            fprintf(stderr, "%s\n", str);
            free(str);
        }
        execute_job(resque, json);
        json_decref(json);
    }

    return 0;
}

static void
timer_cb(EV_P_ ev_timer *w, int revents)
{
    resque_t *resque = w->data;

    if (resque->job_running) {
        return;    
    }

    /* TODO support multiple queues */
    redis_client_lpop(resque->redis, 
                      "resque:queue:basic", 
                      redis_deferred_create(resque->redis, lpop_cb, resque));
}

resque_error_t*
resque_worker(resque_t *resque)
{
    /* check every N seconds */
    ev_timer_init(&resque->timer, timer_cb, resque->interval, resque->interval);
    resque->timer.data = resque;
    ev_timer_start(EV_DEFAULT_ &resque->timer);
    
    /* register redis client */
    resque->redis = redis_client_create(NULL, 0, EV_DEFAULT);

    ev_loop(EV_DEFAULT_ 0);
    return RESQUE_SUCCESS;
}


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
                     redis_deferred_create(client, cb_get, NULL));

    ev_loop(loop, 0);

    return 0;
}


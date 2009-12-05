#include <ev.h>
#include "redis-client.h"

int main()
{
    redis_client_t *client = redis_client_create(NULL, 0);
    return 0;
}

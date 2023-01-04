#include "devices.h"

#include <unistd.h>


void *easy_clock_f(device *d, void *arg)
{
    LOG("starting easy clock");
    int *period = (int *) arg;
    while (1)
    {
        usleep(period);
        for (int i = 0; i < d->conns->count; ++ i)
        {
            d->conns->c[i].value ^= 1;
        }
    }
    return NULL;
}


device *create_easy_clock()
{
    device *c = create_device(easy_clock_f, NULL);
    return c;
}


int main()
{
    return 0;
}

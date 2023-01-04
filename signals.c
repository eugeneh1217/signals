#include "devices.h"

#include <unistd.h>
#include <time.h>


void *easy_clock_f(void *dev, void *arg)
{
    LOG("starting easy clock");
    device *d = (device *) dev;
    int *period = (int *) arg;
    while (1)
    {
        usleep(*period * 1000);
        for (int i = 0; i < d->conns->count; ++ i)
        {
            d->conns->c[i]->value ^= 1;
        }
    }
    return NULL;
}


device *create_easy_clock(int ms_delay)
{
    int *p = malloc(sizeof(int));
    *p = ms_delay;
    device *c = create_device(easy_clock_f, p);
    return c;
}


void destroy_easy_clock(device *ec)
{
    free(ec->state);
    destroy_device(ec);
}


typedef struct 
{
    int ms_period;
    FILE *f;
} writer_state;


void *writer_f(void *dev, void *state)
{
    LOG("starting writer");
    device *d = (device *) dev;
    writer_state *s = (writer_state *) state;
    double elapsed;
    fprintf(s->f, "t, %s", d->conns->c[0]->name);
    for (int i = 1; i < d->conns->count; ++ i)
    {
        fprintf(s->f, ", %s", d->conns->c[i]->name);
    }
    fprintf(s->f, "\n");
    clock_t start = clock();
    while (1)
    {
        elapsed = ((double)(clock() - start)) * 1000 / CLOCKS_PER_SEC;
        fprintf(s->f, "%0.8f, %d", elapsed, d->conns->c[0]->value);
        for (int i = 1; i < d->conns->count; ++ i)
        {
            fprintf(s->f, ", %d", d->conns->c[i]->value);
        }
        fprintf(s->f, "\n");
        usleep(s->ms_period * 1000);
    }
    return NULL;
}


device *create_writer(int ms_delay, FILE *f)
{
    writer_state *s = malloc(sizeof(writer_state));
    s->ms_period = ms_delay;
    s->f = f;
    device *d = create_device(writer_f, s);
    return d;
}


void destroy_writer(device *wr)
{
    free(wr->state);
    destroy_device(wr);
}


pthread_t start_device_thread(device *d)
{
    pthread_t t;
    pthread_create(&t, NULL, run_device, d);
    return t;
}


int main()
{
    FILE *signaltap = fopen("signaltap.csv", "w");

    device *clk = create_easy_clock(100);
    device *wr = create_writer(50, signaltap);

    pthread_t clk_thread = start_device_thread(clk);
    pthread_t wr_thread = start_device_thread(wr);

    cgroup *cs = create_cgroup();
    create_connection(cs, 0, "c0");

    add_connection(clk, cs->c[0]);
    add_connection(wr, cs->c[0]);

    sleep(2);

    // pthread_join(clk_thread, NULL);
    // pthread_join(wr_thread, NULL);
    pthread_cancel(clk_thread);
    pthread_cancel(wr_thread);

    pthread_join(clk_thread, NULL);
    pthread_join(wr_thread, NULL);

    destroy_writer(wr);
    destroy_easy_clock(clk);

    destroy_cgroup(cs);

    fclose(signaltap);
    return 0;
}

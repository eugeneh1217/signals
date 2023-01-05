#include "devices.h"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>


#define NANOTOMILLI 1000000


long long getms(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}


void *easy_clock_f(void *dev, void *arg)
{
    device *d = (device *) dev;
    int *ms_period = (int *) arg;
    LOG("starting easy clock with %dms period", *ms_period);
    struct timespec delay = {0};
    struct timespec rem = {0};
    while (1)
    {
        delay.tv_sec = 0;
        delay.tv_nsec = *ms_period * NANOTOMILLI / 2;
        while (delay.tv_nsec != 0 || delay.tv_sec != 0)
        {
            nanosleep(&delay, &rem);
            memcpy(&delay, &rem, sizeof(struct timespec));
        }
        for (int i = 0; i < d->conns->count; ++ i)
        {
            if (d->conns->c[i]->value == 1)
            {
                d->conns->c[i]->value = 0;
            }
            else
            {
                d->conns->c[i]->value = 1;
            }
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


typedef struct dutyclock_state
{
    int dutypercent;
    int ms_period;
} dutyclock_state;


void *dutyclock_f(void *dev, void *state)
{
    dutyclock_state *dcs = (dutyclock_state *) state;
    device *d = (device *) dev;

    int hdelay = ((double) dcs->dutypercent) / 100 * dcs->ms_period * NANOTOMILLI;
    int ldelay = (dcs->ms_period * NANOTOMILLI) - hdelay;

    struct timespec delay = {0};
    struct timespec rem = {0};

    LOG("starting dutyclock with %d%% duty cycle and %dms period", dcs->dutypercent, dcs->ms_period);

    while (1)
    {
        for (int i = 0; i < d->conns->count; ++ i)
        {
            d->conns->c[i]->value = 1;
        }
        delay.tv_sec = 0;
        delay.tv_nsec = hdelay;
        while (delay.tv_nsec != 0 || delay.tv_sec != 0)
        {
            nanosleep(&delay, &rem);
            memcpy(&delay, &rem, sizeof(struct timespec));
        }
        for (int i = 0; i < d->conns->count; ++ i)
        {
            d->conns->c[i]->value = 0;
        }
        delay.tv_sec = 0;
        delay.tv_nsec = ldelay;
        while (delay.tv_nsec != 0 || delay.tv_sec != 0)
        {
            nanosleep(&delay, &rem);
            memcpy(&delay, &rem, sizeof(struct timespec));
        }
    }

    return NULL;
}


device *create_dutyclock(int dutypercent, int ms_period)
{
    dutyclock_state *dcs = malloc(sizeof(dutyclock_state));
    dcs->dutypercent = dutypercent;
    dcs->ms_period = ms_period;
    device *dclk = create_device(dutyclock_f, dcs);
    return dclk;
}

void destroy_dutyclock(device *dclk)
{
    free(dclk->state);
    destroy_device(dclk);
}


typedef struct msg
{
    char *data;
    size_t size;
} msg;


typedef struct writer_state
{
    int ms_period;
    FILE *f;
} writer_state;


void *writer_f(void *dev, void *state)
{
    device *d = (device *) dev;
    writer_state *s = (writer_state *) state;
    double elapsed;

    LOG("starting writer with %dms sample interval", s->ms_period);

    fprintf(s->f, "t,%s", d->conns->c[0]->name);
    for (int i = 1; i < d->conns->count; ++ i)
    {
        fprintf(s->f, ",%s", d->conns->c[i]->name);
    }
    fprintf(s->f, "\n");

    struct timespec delay = {0}, rem = {0};
    double start = getms();
    while (1)
    {
        elapsed = getms() - start;
        fprintf(s->f, "%0.4f,%d", elapsed, d->conns->c[0]->value);
        for (int i = 1; i < d->conns->count; ++ i)
        {
            fprintf(s->f, ",%d", d->conns->c[i]->value);
        }
        fprintf(s->f, "\n");
        delay.tv_sec = 0;
        delay.tv_nsec = s->ms_period * NANOTOMILLI;
        while (delay.tv_nsec != 0 || delay.tv_sec != 0)
        {
            nanosleep(&delay, &rem);
            memcpy(&delay, &rem, sizeof(struct timespec));
        }
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


int main(int argc, char **argv)
{
    char mode[64] = "easyclock";

    if (argc == 2)
    {
        sprintf(mode, "%s", argv[1]);
    }
    
    FILE *signaltap = fopen("signaltap.csv", "w");

    if (strcmp(mode, "easyclock") == 0)
    {
        LOG("RUNNING EASYCLOCK DEMO");
        device *clk = create_easy_clock(1000);
        device *wr = create_writer(5, signaltap);

        cgroup *cs = create_cgroup();
        create_connection(cs, 0, "c0");

        add_connection(clk, cs->c[0]);
        add_connection(wr, cs->c[0]);

        pthread_t clk_thread = start_device_thread(clk);
        pthread_t wr_thread = start_device_thread(wr);

        sleep(8);

        pthread_cancel(clk_thread);
        pthread_cancel(wr_thread);

        pthread_join(clk_thread, NULL);
        pthread_join(wr_thread, NULL);

        destroy_writer(wr);
        destroy_easy_clock(clk);

        destroy_cgroup(cs);
    }
    else if (strcmp(mode, "dutyclock") == 0)
    {
        LOG("RUNNING DUTYCLOCK DEMO");

        device *dc = create_dutyclock(20, 1000);
        device *wr = create_writer(5, signaltap);

        cgroup *cs = create_cgroup();
        create_connection(cs, 0, "c0");

        add_connection(dc, cs->c[0]);
        add_connection(wr, cs->c[0]);

        pthread_t dc_thread = start_device_thread(dc);
        pthread_t wr_thread = start_device_thread(wr);

        sleep(8);

        pthread_cancel(dc_thread);
        pthread_cancel(wr_thread);

        pthread_join(dc_thread, NULL);
        pthread_join(wr_thread, NULL);

        destroy_writer(wr);
        destroy_dutyclock(dc);

        destroy_cgroup(cs);
    }
    

    fclose(signaltap);
    return 0;
}

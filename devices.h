#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LOGGING
#ifdef LOGGING
#define LOG(format, ...) printf("LOG: " format "\n" __VA_OPT__(,) __VA_ARGS__)
#else
#define LOG(format,...) ;
#endif

#define CONNECTION_INIT {0, {0}, PTHREAD_MUTEX_INITIALIZER}

typedef struct connection
{
    int value;
    pthread_mutex_t mutex;
    char name[64];
} connection;


typedef struct cgroup
{
    connection **c;
    size_t count;
    size_t _size;
} cgroup;


typedef void *(*devicefunc)(void *dev, void *state);


typedef struct device
{
    cgroup *conns;
    devicefunc f;
    void *state;
} device;


cgroup *create_cgroup();
void destroy_cgroup(cgroup *g);
connection *get_connection(cgroup *group, char *name);
// int cg_add_connection(cgroup *group, connection *conn);
connection *create_connection(cgroup *group, int value, char *name);


device *create_device(devicefunc f, void *init_state);
void destroy_device(device *d);
int add_connection(device *d, connection *c);
void *run_device(void *d);

#endif

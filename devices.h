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

struct connection_struct
{
    int value;
    char name[64];
    pthread_mutex_t mutex;
} CONNECTION_INIT = {0, {0}, PTHREAD_MUTEX_INITIALIZER};


typedef struct connection_struct connection;


typedef struct
{
    connection *c;
    size_t count;
    size_t _size;
} cgroup;


typedef void *(*devicefunc)(device *d, void *state);


typedef struct
{
    cgroup *conns;
    devicefunc f;
    void *state;
} device;


cgroup *create_cgroup();
void destroy_cgroup(cgroup *g);
connection *get_connection(cgroup *group, char *name);
int add_connection(cgroup *group, connection *conn);


device *create_device(devicefunc f, void *init_state);
void destroy_device(device *d);


#endif

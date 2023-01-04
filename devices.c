#include "devices.h"


cgroup *create_cgroup()
{
    cgroup *g = malloc(sizeof(cgroup));
    g->count = 0;
    g->_size = 4;
    g->c = calloc(sizeof(connection *), g->_size);
    return g;
}


void destroy_cgroup(cgroup *g)
{
    for (int i = 0; i < g->count; ++ i)
    {
        free(g->c[i]);
    }
    free(g->c);
    free(g);
}


connection *get_connection(cgroup *group, char *name)
{
    for (int i = 0; i < group->count; ++ i)
    {
        if (strcmp(group->c[i]->name, name) == 0)
        {
            return group->c[i];
        }
    }
    return NULL;
}


int cg_add_connection(cgroup *group, connection *conn)
{
    if (group->count == group->_size)
    {
        group->_size *= 2;
        group->c = realloc(group->c, group->_size * sizeof(connection *));
        if (group->c == NULL)
        {
            return -1;
        }
    }
    connection **new_conn = &group->c[group->count ++];
    *new_conn = conn;
    return 0;
}

connection *create_connection(cgroup *group, int value, char *name)
{
    connection *c = malloc(sizeof(connection));
    c->value = value;
    strncpy(c->name, name, 64);
    pthread_mutex_init(&c->mutex, NULL);
    cg_add_connection(group, c);
    return c;
}


device *create_device(devicefunc f, void *init_state)
{
    device *d = malloc(sizeof(device));
    d->conns = create_cgroup();
    d->f = f;
    d->state = init_state;
    return d;
}


void destroy_device(device *d)
{
    // destroy_cgroup(d->conns);
    free(d->conns->c);
    free(d->conns);
    free(d);
}


int add_connection(device *d, connection *c)
{
    return cg_add_connection(d->conns, c);
}


void *run_device(void *d)
{
    device *dev = (device *) d;
    return dev->f(dev, dev->state);
}

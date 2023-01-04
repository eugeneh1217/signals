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


int add_connection(cgroup *group, connection *conn)
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
    destroy_cgroup(d->conns);
    free(d);
}


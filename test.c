#include "devices.h"

#include <assert.h>

int main()
{
    cgroup *g = create_cgroup();

    connection a = (connection) {0, "c0"};
    connection b = (connection) {0, "c1"};
    connection c = (connection) {0, "c2"};

    add_connection(g, &a);
    add_connection(g, &b);
    add_connection(g, &c);

    connection *res = get_connection(g, "c0");
    assert(res != NULL);

    res = get_connection(g, "c1");
    assert(res != NULL);

    res = get_connection(g, "c2");
    assert(res != NULL);

    res = get_connection(g, "c3");
    assert(res == NULL);

    a.value = 42;
    assert(get_connection(g, "c0")->value == 42);

    destroy_cgroup(g);

    printf("all tests passed\n");
    return 0;
}

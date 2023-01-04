#include "devices.h"

#include <assert.h>

int main()
{
    cgroup *g = create_cgroup();

    create_connection(g, 0, "c0");
    create_connection(g, 0, "c1");
    create_connection(g, 0, "c2");

    connection *res = get_connection(g, "c0");
    assert(res != NULL);

    res = get_connection(g, "c1");
    assert(res != NULL);

    res = get_connection(g, "c2");
    assert(res != NULL);

    res = get_connection(g, "c3");
    assert(res == NULL);

    // a.value = 42;
    // assert(get_connection(g, "c0")->value == 42);

    destroy_cgroup(g);

    printf("all tests passed\n");
    return 0;
}

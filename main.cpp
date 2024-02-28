#include <iostream>
#include <enet/enet.h>

int main() {
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);
}

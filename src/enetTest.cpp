#include "enetTest.h"

#define ENET_IMPLEMENTATION
#include <enet.h>

int enetTest::ent()
{
    if (enet_initialize() != 0)
    {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }

    enet_deinitialize();
    return 0;
}
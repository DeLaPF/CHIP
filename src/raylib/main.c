#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "emu.h"

int main(int argc, char *argv[])
{
    // Handle args
    if (argc < 2) {
        printf("Error: specify ROM PATH");
        return 0;
    }
    char* romPath = argv[1];
    bool startPaused = argc > 2 && argv[2][0] == 'p';

    // Init rand to current time
    srand((unsigned int)time(NULL));

    run(romPath, startPaused);

    return 0;
}

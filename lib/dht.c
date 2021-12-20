#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/dht.h"
#include "../include/overlay.h"
#include "../include/underlay.h"

void initDHT(int PORT, int WORKERS){
    if (fork() == 0){
        // TODO
        overlayLoop(PORT, WORKERS);
    }
    else {
        underlayLoop(PORT, WORKERS);
    }
}
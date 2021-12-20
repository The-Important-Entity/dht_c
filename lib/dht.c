#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/dht.h"
#include "../include/overlay.h"
#include "../include/underlay.h"

void initDHT(){
    if (fork() == 0){
        overlayLoop(2500);
    }
    else {
        underlayLoop(2000);
    }
}
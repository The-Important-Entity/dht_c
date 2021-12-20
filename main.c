#include "include/dht.h"
#include "include/utils.h"

void printUsage(char * name){
    printf("Usage: %s <PORT> <NUM_WORKERS>\n", name);
}

void printPortError() {
    printf("Error: Expected an integer greater than 1023 for PORT\n");
}

void printWorkersError() {
    printf("Error: Expected an integer greater than 0 for NUM_WORKERS\n");
}

int main(int argc, char * argv[]){;
    if (argc == 1) {
        printUsage(argv[0]);
        return 0;
    }
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    int PORT;
    if ((PORT = atoi(argv[1])) <= 0 || PORT <= 1023) {
        printPortError();
        printUsage(argv[0]);
        return 1;
    }

    int WORKERS;
    if ((WORKERS = atoi(argv[2])) <= 0) {
        printWorkersError();
        printUsage(argv[0]);
        return 1;
    }
    initDHT(PORT, WORKERS);
}
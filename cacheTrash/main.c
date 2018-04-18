#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 4194304

int main(int argc, const char **argv) {
    srand(time(NULL));
    char *buf = malloc(MAX_SIZE);
    memset(buf, 0, MAX_SIZE);

    for(int i = 0; i < MAX_SIZE; i++) {
        int idx = rand() % MAX_SIZE;
        buf[idx]++;
    }

	return EXIT_SUCCESS;
}

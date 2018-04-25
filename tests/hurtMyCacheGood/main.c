#include <stdlib.h>

int main(int argc, const char **argv) {
	int numBlocks = 0x1000;
	int blockSize = 64;
	char *causingProblems = malloc(sizeof(char) * blockSize * numBlocks);

    int a = 0;

	for (int j = 0; j < 10; j++) {
		for (int k = 0; k < 0x2000; k++) {
			for (int i = 0; i < 32; i++) {
				a = causingProblems[(blockSize * numBlocks / 32 * i) + k];
			}
		}
	}
//	for (int j = 0; j < blockSize; j ++) {
//		for (int i = 0; i < numBlocks * 10; i++) {
//			causingProblems[((3719 * i) % numBlocks) * blockSize + j]++;
//		}
//	}

	free(causingProblems);

	return EXIT_SUCCESS;
}

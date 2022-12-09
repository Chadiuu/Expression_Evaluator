#include<stdlib.h>
#include "memManager.h"

void* allocateMem(void* inputArray, size_t prevSize, size_t newSize) {
	if (newSize == 0) {
		free(inputArray);
		return NULL;
	}
	void* result = realloc(inputArray, newSize); if (result == NULL) exit(2);
	return result;
}
#pragma once
#ifndef memManager_h
#define memManager_h

#include "constants.h"

#define INCREASE_SIZE(size) ((size) < 16 ? 16 : (size)*2)
#define UPDATE_ARRAY(type, inputArray, oldCount, updatedCount) ((type*) allocateMem(inputArray,sizeof(type) * (oldCount),sizeof(type)*(updatedCount)))
#define REMOVE_ARRAY(type,inputArray,oldSize) allocateMem(inputArray,sizeof(type)*oldSize,0)
void* allocateMem(void* inputArray, size_t prevSize, size_t newSize); //reallocate()



#endif // !memManager_h

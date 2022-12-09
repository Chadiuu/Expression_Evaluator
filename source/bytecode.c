#include<stdlib.h>
#include "bytecode.h"
#include "memManager.h"


void intitializeChunk(opChunk* inputChunk) {
	inputChunk->InputCode = NULL;
	initializeArrayValue(&inputChunk->constants_Values);
	inputChunk->size = 0;
	inputChunk->count = 0;
	inputChunk->line = NULL;
}
void removeChunk(opChunk* inputChunk){
	REMOVE_ARRAY(uint8_t, inputChunk->InputCode, inputChunk->size);
	REMOVE_ARRAY(int, inputChunk->line, inputChunk->size);
	removeValueArray(&inputChunk->constants_Values);
	intitializeChunk(inputChunk);
}
void addByteToChunk(opChunk* inputChunk, uint8_t byteToAdd, int line) {
	if (inputChunk->size < inputChunk->count + 1) {
		int oldSize = inputChunk->size;
		inputChunk->size = INCREASE_SIZE(oldSize);
		inputChunk->InputCode = UPDATE_ARRAY(uint8_t, inputChunk->InputCode, 
			oldSize, inputChunk->size);
		inputChunk->line = UPDATE_ARRAY(int, inputChunk->line,
			oldSize, inputChunk->size);

	}
	inputChunk->InputCode[inputChunk->count] = byteToAdd; 
	inputChunk->line[inputChunk->count] = line;
	inputChunk->count++;
}\

int addConstantToPool(opChunk* inputChunk, literalValue value)
{
	addValueArray(&inputChunk->constants_Values, value);
	return inputChunk->constants_Values.count - 1;
}

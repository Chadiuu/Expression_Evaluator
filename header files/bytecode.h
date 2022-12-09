#pragma once
#ifndef bytecode_h
#define bytecode_h
#include "constants.h"
#include "representValues.h"
typedef enum {
	OPCODE_CONSTANT,
	OPCODE_NIL,
	OPCODE_TRUE,
	OPCODE_FALSE,
	OPCODE_EQUAL,
	OPCODE_GREATER,
	OPCODE_LESS,
	OPCODE_NEGATE,
	OPCODE_ADD,
	OPCODE_SUBSTRACT,
	OPCODE_MULTIPLY,
	OPCODE_DIVIDE,
	OPCODE_NOT,
	OPCODE_RETURN,
}OPCODE;

typedef struct {
	uint8_t* InputCode; //code
	const_pool_value constants_Values; // constants
	int count;
	int size; //capacity
	int* line;
}opChunk; //Chunk
void intitializeChunk(opChunk* inputChunk);
void removeChunk(opChunk* inputChunk); // freeChunk();
void addByteToChunk(opChunk* inputChunk, uint8_t byteToAdd, int line); // writeChunk()
int addConstantToPool(opChunk* inputChunk, literalValue value); // addConstant();




#endif // !bytecode_h

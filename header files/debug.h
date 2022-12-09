#pragma once
#ifndef debug_h
#define debug_h
#include "bytecode.h"


void disassembleByteCode(opChunk* chunk, const char* name);
int disassembleInstruction(opChunk* chunk, int offset);

#endif // !debug_h

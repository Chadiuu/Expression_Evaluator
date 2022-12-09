#pragma once
#ifndef compiler
#define compiler
#include "bytecode.h"
bool compile_code(const char* code, opChunk* chunk);
#endif // !compiler

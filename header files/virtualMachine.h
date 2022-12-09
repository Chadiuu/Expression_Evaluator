#pragma once
#ifndef vm_c
#define vm_c
#define MAX_SIZE 256 // STACK_MAX
#include "bytecode.h"
#include "representValues.h"
typedef struct 
{
	opChunk* userCode; // chunk
	uint8_t* instrution_pointer; //ip
	literalValue stack[MAX_SIZE];
	literalValue* topOfStack; //stackTop
}virtual_machine;
typedef enum 
{
	COMPILE_TIME_ERROR,
	RUNTIME_ERROR,
	NO_ERROR

}result_vm;
void initializeVM();
void deleteVM();
result_vm run_code_vm(const char* inputCode); // interpret
void push_stack(literalValue value);
literalValue pop_stack();


#endif // !vm

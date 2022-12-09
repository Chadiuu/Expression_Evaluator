#include "constants.h"
#include "representValues.h"
#include "virtualMachine.h"
#include <stdio.h>
#include "debug.h"
#include "compiler.h"
#include <stdarg.h>
virtual_machine vm;
static void reset_stack() {
	vm.topOfStack = vm.stack;
}
static void throwRunTimeError(const char* format, ...) {
	va_list arguments;
	va_start(arguments,format);
	vfprintf(stderr, format, arguments);
	va_end(arguments);
	fputs("\n",stderr);
	size_t byte_instrction = vm.instrution_pointer - vm.userCode->InputCode - 1;
	int line = vm.userCode->line[byte_instrction];
	fprintf(stderr, "[line %d] in ...\n", line);
	reset_stack();
}
void initializeVM() {
	reset_stack();
}
void deleteVM() {

}
void push_stack(literalValue value) {
	*vm.topOfStack = value;
	vm.topOfStack++;
}
literalValue pop_stack() {
	vm.topOfStack--;
	return *vm.topOfStack;
}
static literalValue peek(int distance) {
	return vm.topOfStack[-1 - distance];
}
static bool is_false(literalValue val) {
	return CHECK_NIL(val) || (CHECK_BOOL(val) && !STRUCT_BOOL(val));
}
static result_vm run_code() { // run()
	#define READ_INSTRUCTION()(*vm.instrution_pointer++) // READ_BYTE()
	#define READ_CONSTANT_VALUE()(vm.userCode->constants_Values.Values[READ_INSTRUCTION()])
	#define DO_BINARY_OPERATION(value_type,op) \
			do{\
				if(!CHECK_NUMBER(peek(0)) || !CHECK_NUMBER(peek(1))){\
					throwRunTimeError("NAN");\
					return RUNTIME_ERROR;\
				}\
			double second = STRUCT_NUMBER(pop_stack());\
			double first = STRUCT_NUMBER(pop_stack());\
			push_stack(value_type(first op second));\
			} while (false)

	for (;;) {
		#ifdef TRACE
		printf("          ");
		for (literalValue* val = vm.stack; val < vm.topOfStack; val++)
		{
			printf("[ ");
			print_value(*val);
			printf(" ]");
		}
		printf("\n");
		disassembleInstruction(vm.userCode, (int)(vm.instrution_pointer - vm.userCode->InputCode));
		#endif
		uint8_t byte; // instruction
		switch (byte = READ_INSTRUCTION())
		{
		case OPCODE_RETURN:
			print_value(pop_stack());
			printf("\n");
			return NO_ERROR;
		case OPCODE_CONSTANT: {
			literalValue constant_value; // constant
			constant_value = READ_CONSTANT_VALUE(); // READ_CONSTANT
			push_stack(constant_value);
			break;
		}
			
		case OPCODE_NEGATE:{
			if (!CHECK_NUMBER(peek(0))) {
			throwRunTimeError("NAN");
			return RUNTIME_ERROR;
		}
						  push_stack(NUMBER_STRUCT(-STRUCT_NUMBER(pop_stack())));
						  break; 
		}
			
		case OPCODE_NIL: push_stack(NIL_STRUCT); break;
		case OPCODE_TRUE: push_stack(BOOL_STRUCT(true)); break;
		case OPCODE_FALSE: push_stack(BOOL_STRUCT(false)); break;	
		case OPCODE_EQUAL: {
			literalValue second = pop_stack();
			literalValue first = pop_stack();
			push_stack(BOOL_STRUCT(is_value_equal(first, second))); break;
		}
		case OPCODE_GREATER: DO_BINARY_OPERATION(BOOL_STRUCT, >); break;
		case OPCODE_LESS: DO_BINARY_OPERATION(BOOL_STRUCT, < ); break;
		case OPCODE_ADD: DO_BINARY_OPERATION(NUMBER_STRUCT,+);break;
		case OPCODE_SUBSTRACT:DO_BINARY_OPERATION(NUMBER_STRUCT,-);break;
		case OPCODE_MULTIPLY:DO_BINARY_OPERATION(NUMBER_STRUCT,*);break;
		case OPCODE_DIVIDE:DO_BINARY_OPERATION(NUMBER_STRUCT,/);break;
		case OPCODE_NOT: push_stack(BOOL_STRUCT(is_false(pop_stack())));
		default:
			break;
		}
	}
	#undef READ_INSTRUCTION
	#undef READ_CONSTANT_VALUE
	#undef DO_BINARY_OPERATION
}
result_vm run_code_vm(const char* inputCode)
{
	opChunk chunk;
	intitializeChunk(&chunk);
	if (!compile_code(inputCode, &chunk)) {
		removeChunk(&chunk);
		return COMPILE_TIME_ERROR;
	}
	
	vm.userCode = &chunk;
	vm.instrution_pointer = vm.userCode->InputCode;
	result_vm res = run_code();
	removeChunk(&chunk);
	return res;
}

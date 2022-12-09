#include<stdio.h>
#include "debug.h"
#include "representValues.h"
void disassembleByteCode(opChunk* chunk, const char* name) // dissasembleChunk()
{
	printf("== %s ==\n", name);
	for (int offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset);
	}
}
static int constInt(const char* name, opChunk* chunk, int offset) {
	uint8_t constant = chunk->InputCode[offset + 1];
	printf("%-16s %4d '", name, constant);
	print_value(chunk->constants_Values.Values[constant]);
	printf("\n");
	return offset + 2;
}
static int simpleInstruct(const char* name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}
int disassembleInstruction(opChunk* chunk, int offset)
{
	printf("%04d ", offset);
	if (offset > 0 && chunk->line[offset] == chunk->line[offset - 1]) {
		printf("   |");
	}
	else
	{
		printf("%4d ", chunk->line[offset]);
	}
	uint8_t instruction = chunk->InputCode[offset];
	switch (instruction)
	{
		case OPCODE_CONSTANT:
		return constInt("OPCODE_CONSTANT", chunk, offset);
		case OPCODE_NIL: return simpleInstruct("OPCODE_NIL", offset);
		case OPCODE_TRUE: return simpleInstruct("OPCODE TRUE", offset);
		case OPCODE_FALSE: return simpleInstruct("OPCODE FALSE", offset);
		case OPCODE_EQUAL: return simpleInstruct("OPCODE EQUAL", offset);
		case OPCODE_GREATER: return simpleInstruct("OPCODE GREATER", offset);
		case OPCODE_LESS: return simpleInstruct("OPCODE_LESS", offset);

		case OPCODE_RETURN:
			return simpleInstruct("OPCODE_RETURN", offset);
		case OPCODE_NEGATE:
			return simpleInstruct("OPCODE_NEGATE", offset);
		case OPCODE_ADD:
			return simpleInstruct("OPCODE_ADD", offset);
		case OPCODE_SUBSTRACT:
			return simpleInstruct("OPCODE_SUBS", offset);
		case OPCODE_MULTIPLY:
			return simpleInstruct("opcode mult", offset);
		case OPCODE_DIVIDE:
			return simpleInstruct("opdiv", offset);
		case OPCODE_NOT: return simpleInstruct("OPCODE_NOT", offset);
		default:
			printf("invalid opode %d\n", instruction);
			break;
	}

}
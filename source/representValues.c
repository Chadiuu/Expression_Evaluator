#pragma once
#include <stdio.h>
#include "memManager.h"
#include "representValues.h"

void initializeArrayValue(const_pool_value* inputArray)
{
	inputArray->Values = NULL;
	inputArray->size = 0;
	inputArray->count = 0;
}

void addValueArray(const_pool_value* inputArray, literalValue value)
{
	if (inputArray->size < inputArray->count + 1) {
		int oldSize = inputArray->size;
		inputArray->size = INCREASE_SIZE(oldSize);
		inputArray->Values = UPDATE_ARRAY(literalValue, inputArray->Values, oldSize, inputArray->size);
	}

	inputArray->Values[inputArray->count] = value; inputArray->count++;
}

void removeValueArray(const_pool_value* inputArray)
{
	REMOVE_ARRAY(literalValue, inputArray->Values, inputArray->size);
	initializeArrayValue(inputArray);
}
void print_value(literalValue val) {
	switch (val.type)
	{
	case BOOL:
		printf(STRUCT_BOOL(val) ? "TRUE" : "false");
		break;
	case NIL: printf("NIL"); break;
	case NUMBER: printf("%g",STRUCT_NUMBER(val)); break;
	default:
		break;
	}
}
bool is_value_equal(literalValue first, literalValue second) {
	if (first.type != second.type) {
		return false;
	}
	switch (first.type)
	{
	case BOOL: return STRUCT_BOOL(first) == STRUCT_BOOL(second);
	case NIL: return true;
	case NUMBER: return STRUCT_NUMBER(first) == STRUCT_NUMBER(second);
	default:
		return false;
	}
}
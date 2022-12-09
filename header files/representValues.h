#pragma once
#include "constants.h"

typedef enum 
{
	BOOL,NIL,NUMBER
}val_type;// ValueType
typedef struct 
{
	val_type type;
	union 
	{
		bool boolean;
		double number;
	}value_as; //as

}literalValue; // Value
#define CHECK_BOOL(value)   ((value).type == BOOL) //IS_BOOl
#define CHECK_NIL(value)    ((value).type == NIL) //IS_NIl
#define CHECK_NUMBER(value) ((value).type == NUMBER) //IS_NUMBER

#define STRUCT_BOOL(value) ((value).value_as.boolean)//AS_BOOL
#define STRUCT_NUMBER(value) ((value).value_as.number)//AS_NUMBER
#define BOOL_STRUCT(value) ((literalValue){BOOL,{.boolean = value}}) //BOOL_VAL
#define NIL_STRUCT ((literalValue){NIL,{.number = 0}}) // NIL_VAL
#define NUMBER_STRUCT(value) ((literalValue){NUMBER,{.number = value}}) //NUMBER_VAl

typedef struct {
	literalValue* Values;
	int size; //capacity
	int count;
}const_pool_value; // ValueArray;
bool is_value_equal(literalValue first, literalValue second); // valuesEqual

void initializeArrayValue(const_pool_value* inputArray); // 
void addValueArray(const_pool_value* inputArray, literalValue value);//writeValueArray();
void removeValueArray(const_pool_value* inputArray); //freeValueArray()
void print_value(literalValue val);

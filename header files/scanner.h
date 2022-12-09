#pragma once
#ifndef scanner_h
#define scanner_h
typedef enum  {
	LEFT_PARAN_TOKEN,
	RIGHT_PARAN_TOKEN,
	LEFT_BRACE_TOKEN,
	RIGHT_BRACE_TOKEN,
	COMMA_TOKEN,
	DOT_TOKEN,
	MINUS_TOKEN,
	PLUS_TOKEN,
	SEMICOLON_TOKEN,
	SLASH_TOKEN,
	STAR_TOKEN,
	BANG_TOKEN,
	BANG_EQUAL_TOKEN,
	EQUAL_TOKEN,
	EQUAL_EQUAL_TOKEN,
	GREATER_TOKEN,
	GREATER_EQUAL_TOKEN,
	LESS_TOKEN,
	LESS_EQUAL_TOKEN,
	INDENTIFIER_TOKEN,
	STRING_TOKEN,
	NUMBER_TOKEN,
	AND_TOKEN,
	CLASS_TOKEN,
	ELSE_TOKEN,
	FALSE_TOKEN,
	FOR_TOKEN,
	FUN_TOKEN,
	IF_TOKEN,
	NIL_TOKEN,
	OR_TOKEN,
	PRINT_TOKEN,
	RETURN_TOKEN,
	SUPER_TOKEN,
	THIS_TOKEN,
	TRUE_TOKEN,
	VAR_TOKEN,
	WHILE_TOKEN,
	ERROR_TOKEN,
	EOF_TOKEN,

}token_type;
typedef struct 
{
	token_type type;
	const char* start;
	int length;
	int line;
}Token;

void initialize_scanner(const char* code);
Token scan_token();


#endif // !scanner

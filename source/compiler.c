
#include<stdio.h>
#include<stdlib.h>
#include "constants.h"
#include "compiler.h"
#include "scanner.h"
#include "bytecode.h"
#include "debug.h"



typedef struct {
	Token curr;
	Token prev;
	bool err;
	bool err_casc;
}Parser;
typedef enum 
{
	NONE_PRECEDENCE,
	ASSIGNMENT_PRECEDENCE,
	OR_PRECEDENCE,
	AND_PRECEDENCE,
	EQUALITY_PRECEDENCE,
	COMPARE_PRECEDENCE,
	TERM_PRECEDENCE,
	FACTOR_PRECEDENCE,
	UNARY_PRECEDENCE,
	CALL_PRECEDENCE,
	PRIM_PRECEDENCE


}Precedence;
typedef void (*parse_func)();
typedef struct
{
	parse_func prefix;
	parse_func infix;
	Precedence precedence;
}Parser_Rule;


Parser parser;
opChunk* chunk_compiler;
static opChunk* curr_chunk() {
	return chunk_compiler;
}
static void throwErrorAt(Token* token, const char* err) {
	if (parser.err_casc) return;
	parser.err_casc = true;
	fprintf(stderr, "[line %d] error: ", token->line);
	if (token->type == EOF_TOKEN) { fprintf(stderr, "EOF"); }
	else if (token->type = ERROR_TOKEN) { ; }
	else
	{
		fprintf(stderr, "at '%.*s'", token->length, token->start);
	}
	fprintf(stderr,": %s\n", err);
	parser.err = true;
	
}
static void throwError(const char* err) { // error()
	throwErrorAt(&parser.prev,err);
}
static void reportErrorAt(const char* error) { // errorAtCurrent()
	throwErrorAt(&parser.curr, error);
}
static void advance() {
	parser.prev = parser.curr;
	for (;;) {
		parser.curr = scan_token();
		if (parser.curr.type != ERROR_TOKEN) break;
		reportErrorAt(parser.curr.start);
	}
}

static void advance2(token_type type,const char* err) { //consume
	if (parser.curr.type == type) { advance(); return; }
	reportErrorAt(err);
}
static void addByte(uint8_t byte) {
	addByteToChunk(curr_chunk(), byte, parser.prev.line);
}
static void addMultBytes(uint8_t byte1,uint8_t byte2) {
	addByte(byte1); addByte(byte2); return;
}
static void add_byte_return() {
	addByte(OPCODE_RETURN);
}
static uint8_t returnConstant(literalValue val) {
	int constant = addConstantToPool(curr_chunk(), val);
	if (constant > UINT8_MAX) {
		throwError("constant error"); return 0;
	}
	return (uint8_t)constant;
}
static void add_constant(literalValue val) {
	addMultBytes(OPCODE_CONSTANT, returnConstant(val));
}
static void closeCompiler() {
	add_byte_return();
#ifdef DEBUG_PRINT_CODE
	if (!parser.err)
	{
		disassembleByteCode(curr_chunk(), "code ");
	}
#endif 

}
static void compile_expression();
static Parser_Rule* get_rule(token_type type);
static void parser_precedence(Precedence precedence);

static void compile_binary() {
	token_type type = parser.prev.type;
	Parser_Rule* rule = get_rule(type);
	parser_precedence((Precedence)(rule->precedence + 1));
	switch (type)
	{
	case BANG_EQUAL_TOKEN: addMultBytes(OPCODE_EQUAL, OPCODE_NOT);break;
	case EQUAL_EQUAL_TOKEN: addByte(OPCODE_EQUAL);break;
	case GREATER_TOKEN: addByte(OPCODE_GREATER); break;
	case GREATER_EQUAL_TOKEN: addMultBytes(OPCODE_LESS,OPCODE_NOT); break;
	case LESS_TOKEN: addByte(OPCODE_LESS); break;
	case LESS_EQUAL_TOKEN: addMultBytes(OPCODE_GREATER,OPCODE_NOT); break; // end of session
	case PLUS_TOKEN: addByte(OPCODE_ADD); break;
	case MINUS_TOKEN: addByte(OPCODE_SUBSTRACT); break;
	case STAR_TOKEN: addByte(OPCODE_MULTIPLY); break;
	case SLASH_TOKEN: addByte(OPCODE_DIVIDE); break;
	default:
		return;
	}
}

static void compiler_literal() {
	switch (parser.prev.type)
	{
	case FALSE_TOKEN: addByte(OPCODE_FALSE); break;
	case NIL_TOKEN: addByte(OPCODE_NIL); break;
	case TRUE_TOKEN: addByte(OPCODE_TRUE); break;
	default:
		return;
		break;
	}
}

static void parantheses() {
	compile_expression();
	advance2(RIGHT_PARAN_TOKEN,"expected token: )");
}
static void compile_number() {
	double val = strtod(parser.prev.start,NULL);
	add_constant(NUMBER_STRUCT(val));
}
static void eval_unary() {
	token_type type = parser.prev.type;
	parser_precedence(UNARY_PRECEDENCE);
	switch (type)
	{
	case BANG_TOKEN: addByte(OPCODE_NOT); break;
	case MINUS_TOKEN: addByte(OPCODE_NEGATE); break;
	default:
		return;
	}
}

Parser_Rule rules[] = {
	[LEFT_PARAN_TOKEN]    = {parantheses,  NULL,  NONE_PRECEDENCE},
	[RIGHT_PARAN_TOKEN]   = {NULL,  NULL,  NONE_PRECEDENCE},
	[LEFT_BRACE_TOKEN]    = {NULL,  NULL,  NONE_PRECEDENCE},
	[RIGHT_BRACE_TOKEN]   = {NULL,  NULL,  NONE_PRECEDENCE},
	[COMMA_TOKEN]         = {NULL,  NULL,  NONE_PRECEDENCE},
	[DOT_TOKEN]		      = {NULL,  NULL,  NONE_PRECEDENCE},
	[MINUS_TOKEN]         = {eval_unary,  compile_binary,  TERM_PRECEDENCE},
	[PLUS_TOKEN]	      = {NULL,  compile_binary,  TERM_PRECEDENCE},
	[SEMICOLON_TOKEN]     = {NULL,  NULL,  NONE_PRECEDENCE},
	[SLASH_TOKEN]	      = {NULL,  compile_binary,  FACTOR_PRECEDENCE},
	[STAR_TOKEN]		  = {NULL,  compile_binary,  FACTOR_PRECEDENCE},
	[BANG_TOKEN]		  = {eval_unary,  NULL,  NONE_PRECEDENCE},
	[BANG_EQUAL_TOKEN]    = {NULL,  compile_binary,  EQUALITY_PRECEDENCE},
	[EQUAL_TOKEN]	      = {NULL,  NULL,  NONE_PRECEDENCE},
	[EQUAL_EQUAL_TOKEN]	  = {NULL,  compile_binary,  EQUALITY_PRECEDENCE},
	[GREATER_TOKEN]		  = {NULL,  compile_binary,  COMPARE_PRECEDENCE},
	[GREATER_EQUAL_TOKEN] = {NULL,  compile_binary,  COMPARE_PRECEDENCE},
	[LESS_TOKEN]          = {NULL,  compile_binary,  COMPARE_PRECEDENCE},
	[LESS_EQUAL_TOKEN]    = {NULL,  compile_binary,  COMPARE_PRECEDENCE},
	[INDENTIFIER_TOKEN]   = {NULL,  NULL,  NONE_PRECEDENCE},
	[STRING_TOKEN]        = {NULL,  NULL,  NONE_PRECEDENCE},
	[NUMBER_TOKEN]        = {compile_number,  NULL,  NONE_PRECEDENCE},
	[AND_TOKEN]           = {NULL,NULL,NONE_PRECEDENCE},
	[CLASS_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[ELSE_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[FALSE_TOKEN] = {compiler_literal,NULL,NONE_PRECEDENCE},
	[FOR_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[FUN_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[IF_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[NIL_TOKEN] = {compiler_literal,NULL,NONE_PRECEDENCE},
	[OR_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[PRINT_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[RETURN_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[SUPER_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[THIS_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[TRUE_TOKEN] = {compiler_literal,NULL,NONE_PRECEDENCE},
	[VAR_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[WHILE_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[ERROR_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},
	[EOF_TOKEN] = {NULL,NULL,NONE_PRECEDENCE},


};
static void parser_precedence(Precedence precedence) {
	advance();
	parse_func prefix_rule = get_rule(parser.prev.type)->prefix;
	if (prefix_rule == NULL) {
		throwError("not expression."); return;
	}
	prefix_rule();
	while (precedence<= get_rule(parser.curr.type)->precedence)
	{
		advance();
		parse_func infix_rule = get_rule(parser.prev.type)->infix;
		infix_rule();
	}
}
static Parser_Rule* get_rule(token_type type) {
	return &rules[type];
}
static void compile_expression() {
	parser_precedence(ASSIGNMENT_PRECEDENCE);
	
}
bool compile_code(const char* code, opChunk* chunk) {
	initialize_scanner(code);
	chunk_compiler = chunk;
	parser.err = false;
	parser.err_casc = false;

	advance();
	compile_expression();
	advance2(EOF_TOKEN,"end"); 
	closeCompiler();
	return !parser.err;
	
}

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "scanner.h"



typedef struct
{
	const char* start;
	const char* current;
	int line;
}scanner_t;
scanner_t scanner;
void initialize_scanner(const char* code)
{
	scanner.start = code;
	scanner.current = code;
	scanner.line = 1;
}
static bool alpha(char chr) {
	return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z') || chr == '_';
}
static bool digit(char chr) {
	return chr >= '0' && chr <= '9'; 
}
static bool atEnd() {
	return *scanner.current == '\0'; 
}
static char advance() {
	scanner.current++;
	return scanner.current[-1];
}
static char peek() {
	return *scanner.current; 
}
static char peek2() { 
	if (atEnd()) {
		return '\0';
	}
	else {
		return scanner.current[1];
	}
	}
		
static bool match_token(char chr) {
	if (atEnd())
	{
		return false;
	}
	if (*scanner.current != chr) { 
		return false;
	}
	scanner.current++; 
	return true;
}
static Token make_token(token_type type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.line = scanner.line;
	token.length = (int)(scanner.current - scanner.start);
	return token;
}
static Token throwError(const char* errMsg) {
	Token token;
	token.type = ERROR_TOKEN;
	token.start = errMsg;
	token.length = (int)(strlen(errMsg));
	token.line = scanner.line;
	return token;
}
static void remove_whiteSpace() {
	for (;;) {
		char c = peek();
		switch (c) {
		case' ':
		case '\t':
		case'\r':
			advance(); break;
		case'\n':
			scanner.line++;
			advance();
			break;
		case'/':
			if (peek2() == '/') {
				while (peek() != '\n' && !atEnd())advance;
			}
			else
			{
				return;
			}
			break;
		default:
			return;
		}
	}
}
static token_type ifKeyword(int start,int length,const char* word,token_type type) {
	if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, word, length) == 0) return type;
	return INDENTIFIER_TOKEN;
}
static token_type get_type_identifier() {
	switch (scanner.start[0]) {
	case'a': return ifKeyword(1, 2, "nd", AND_TOKEN);
	case'c': return ifKeyword(1, 4, "lass", CLASS_TOKEN);
	case'e': return ifKeyword(1, 3, "lse", ELSE_TOKEN);
	case'i': return ifKeyword(1, 1, "f", IF_TOKEN);
	case'f': if (scanner.current - scanner.start > 1) {
		switch (scanner.start[1]) {
		case'o': return ifKeyword(2, 1, "r", FOR_TOKEN);
		case'u': return ifKeyword(2, 1, "n", FUN_TOKEN);
		case'a': return ifKeyword(2, 3, "lse", FALSE_TOKEN);
		}
	}
		   break;

	case'n': return ifKeyword(1, 2, "il", NIL_TOKEN);
	case'o': return ifKeyword(1, 1, "r", OR_TOKEN);
	case'p': return ifKeyword(1, 4, "rint", PRINT_TOKEN);
	case'r': return ifKeyword(1, 5, "eturn", RETURN_TOKEN);
	case's': return ifKeyword(1, 4, "uper", SUPER_TOKEN);
	case't': if (scanner.current - scanner.start > 1) {
		switch (scanner.start[1]) {
		case'h': return ifKeyword(2, 2, "is", THIS_TOKEN);
		case'r': return ifKeyword(2, 2, "ue", TRUE_TOKEN);
		}
	} break;
	case'v': return ifKeyword(1, 2, "ar", VAR_TOKEN);
	case'w': return ifKeyword(1, 4, "hile", WHILE_TOKEN);
	}	
	return INDENTIFIER_TOKEN;
}
static Token get_identifier() {
	while (alpha(peek()) || digit(peek())) advance(); 
	return make_token(get_type_identifier());
}
static Token get_number() {

	while (digit(peek()))
	{
		advance();
	}
   if (digit(peek2()) && peek() == '.') {
	advance();
	while (digit(peek())) advance();
    }
   return make_token(NUMBER_TOKEN);
}
static Token get_string() {
	while (peek() != '=' && !atEnd()) {
		if (peek() == '\n') {
			scanner.line++;
		}
		advance();
	} if (atEnd()) return throwError("not finished string"); advance();
	return make_token(STRING_TOKEN);
}

Token scan_token() {
	remove_whiteSpace();
	scanner.start = scanner.current;
	if (atEnd()) return make_token(EOF_TOKEN);
	char c = advance();
	if (alpha(c)) return get_identifier();
	if (digit(c)) return get_number();
	switch (c) {
	case'(': return make_token(LEFT_PARAN_TOKEN);
	case ')': return make_token(RIGHT_PARAN_TOKEN);
	case'{': return make_token(LEFT_BRACE_TOKEN);
	case'}': return make_token(RIGHT_BRACE_TOKEN);
	case';': return make_token(SEMICOLON_TOKEN);
	case',': return make_token(COMMA_TOKEN);
	case'-': return make_token(MINUS_TOKEN);
	case'+': return make_token(PLUS_TOKEN);
	case'/': return make_token(SLASH_TOKEN);
	case'*': return make_token(STAR_TOKEN);
	case'!': return make_token(match_token('=') ? BANG_EQUAL_TOKEN : BANG_TOKEN);
	case'=': return make_token(match_token('=') ? EQUAL_EQUAL_TOKEN : EQUAL_TOKEN);
	case'<': return make_token(match_token('=') ? LESS_EQUAL_TOKEN : LESS_TOKEN);
	case'>': return make_token(match_token('=') ? GREATER_EQUAL_TOKEN : GREATER_TOKEN);
	case'"': return get_string(); // string()
	
	}
	return throwError("thrown error.");
}

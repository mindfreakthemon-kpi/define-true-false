/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>
#include <iostream>
#include "Lexer.h"
#include <ctype.h>
using namespace token;
using namespace lexer;

Lexer::Lexer(std::string source): col(0), row(1), charsDone(-1), file_buffer(source) {
  file_length = source.size();
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;
	bool done = true;
	int c;
	//read character
	while(done){
		charsDone++;
		col++;
		if(charsDone >= file_length)
		{
			//end of file
			return new Token(LT, SourceLocation(row, col+1));
		}
		//for DOS line endings
		if (file_buffer[charsDone] == '\r' && file_buffer[charsDone + 1] == '\n') {
			row++;
			col = 1;
			charsDone = charsDone + 2;
		}
		//for *nix line endings
		if (file_buffer[charsDone] == '\n') {
			row++;
			col = 1;
			charsDone++;
		}
		c = file_buffer[charsDone];
		//remove whitespace
		if(c != ' ')
			done = false;
	}
	
	//some serious stuff
	SourceLocation sl = SourceLocation(row, col);
	switch (c) {
	case '{':
		return new Token(LF_CR_BRACKET, sl);
	case '}':
		return new Token(RT_CR_BRACKET, sl);
	case '(':
		return new Token(LF_PARENTHESES, sl);
	case ')':
		return new Token(RT_PARENTHESES, sl);
	case '[':
		return new Token(LF_SQ_BRACKET, sl);
	case ']':
		return new Token(RT_SQ_BRACKET, sl);
	case ';':
		return new Token(SEMICOLON, sl);
	case ':':
		return new Token(COLON, sl);
	case ',':
		return new Token(COMMA, sl);
	//here goes operators
	case '<':
		return new Token(LESS, sl);
	case '>':
		return new Token(MORE, sl);
	case '+':
		return new Token(PLUS, sl);
	case '-':
		return new Token(MINUS, sl);
	case '*':
		return new Token(MULT, sl);
	case '/':
		return new Token(DIVIDE, sl);
	case '^':
		return new Token(CARET, sl);
	case '=':
		if(file_buffer[charsDone+1] == '='){
			return new Token(DEQUALS, sl);
		}
		else
			return new Token(EQUALS, sl);
	case '&':
		return new Token(AND, sl);
	case '|':
		return new Token(OR, sl);
		//STRING
	case '"': {
		SourceLocation pos(row, col);
		while (file_buffer[++charsDone] != '"') {
			nts.push_back(file_buffer[charsDone]);
		};
		col = col + nts.length();
		return new Token(STRING, pos, nts);
	}
		
	default: {
		while((charsDone < file_length) && (isalnum(file_buffer[charsDone]) != 0 || file_buffer[charsDone] == '_')){
			nts.push_back(file_buffer[charsDone]);
			charsDone++;
			col++;
		}
		//warning, if no token founded this will cause inf loop
		charsDone--;
		col--;
		//check if it keyword
		if(nts.compare("function") == 0)
		{
			return new Token(FUNCTION,sl);
		}
		if(nts.compare("int") == 0)
		{
			return new Token(TYPE_SCALAR,sl,TYPE_INT);
		}
		if(nts.compare("double") == 0)
		{
			return new Token(TYPE_SCALAR,sl,TYPE_DOUBLE);
		}
		if(nts.compare("bool") == 0)
		{
			return new Token(TYPE_SCALAR,sl,TYPE_BOOL);
		}
		if(nts.compare("string") == 0)
		{
			return new Token(TYPE_SCALAR,sl,TYPE_STRING);
		}
		if(nts.compare("void") == 0)
		{
			return new Token(TYPE_SCALAR,sl,TYPE_VOID);
		}
		// TODO more keywords !!!
		//non keyword(id)
		return new Token(ID,sl,nts);
		break;
	}
	}
	return NULL;
}

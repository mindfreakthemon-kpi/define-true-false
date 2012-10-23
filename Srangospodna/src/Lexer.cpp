/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>

#include "Lexer.h"
using namespace token;
using namespace lexer;
Lexer::Lexer(std::string file) {
	file_is.open(file.c_str(), ios::binary);
	file_is.seekg(0, ios::end);
	file_length = file_is.tellg();
	file_is.seekg(0, ios::beg);
	file_buffer = new char[file_length];
	file_is.read(file_buffer, file_length);
	file_is.close();
	col = 0;
	row = 1;
	charsDone = -1;
}

Lexer::Lexer(char[] str) {
	file_buffer = str;
	col = 0;
	row = 1;
	charsDone = -1;
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;

	//read character
	if(charsDone >= file_length)
	{
		return new Token(LT, SourceLocation(row, col+1));
	}
	else
	{
		charsDone++;
		col++;
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
	}

	int c = file_buffer[charsDone];

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
			getNextChar();
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
		break;
	}
	}
	return NULL;
}

int Lexer::getNextChar(bool doIncrement) {
	if (charsDone >= file_length)
	if (doIncrement) {
		charsDone++;
		col++;
		//for DOS line endings
		if (file_buffer[charsDone] == '\r'
				&& file_buffer[charsDone + 1] == '\n') {
			row++;
			col = 0;
			charsDone++;
		}
		//for *nix line endings
		if (file_buffer[charsDone] == '\n') {
			row++;
			col = 0;
		}
	} else {
		return file_buffer[charsDone + 1];//return next char, but not increment charsDone & col & row
	}
	return file_buffer[charsDone];
}
bool Lexer::foundChar(char c, bool allowWhitespace) {
	bool done = false;
	int i = 1;
	while(!done){
		if(allowWhitespace == true){
			if(file_buffer[charsDone+i] == c)
				return true;
			else if(file_buffer[charsDone+i] != ' ')
				return false;//char not found
		}
		else
			if(file_buffer[charsDone+i] != c)
				return false;
		i++;
	}
	return false;
}

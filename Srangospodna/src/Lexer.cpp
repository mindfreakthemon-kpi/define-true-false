/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>

#include "Lexer.h"
using namespace token;
const int Lexer::_EOF = -1050;
const int Lexer::_ENDL = -5010;
Lexer::Lexer(std::string file) {
	file_is.open(file.c_str(), ios::binary);
	file_is.seekg(0, ios::end);
	file_length = file_is.tellg();
	file_is.seekg(0, ios::beg);
	file_buffer = new char[file_length];
	file_is.read(file_buffer, file_length);
	file_is.close();
	col = 1;
	row = 1;
	charsDone = -1;
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;
	
	//read character
	int c = getNextChar();
	
	//some serious stuff
	switch (c) {
	case '{':{
		SourceLocation s(row, col);
		return new Token(LF_CR_BRACKET, s);}
	case '}':
		return new Token(RT_CR_BRACKET, SourceLocation(row, col));
	case '"': {
		SourceLocation pos(row,col);
		do {
			c = getNextChar();
			if (c == '\\') {
				c = getNextChar();
				nts.push_back(c);
				//TODO process escaped symbols
			}
			if (c != '"')
				nts.push_back(c);
		} while (c != '"');
		return new Token(STRING,pos,nts);
	}
	case Lexer::_EOF:
		return new Token(LT,SourceLocation(row, col));
	}
	return NULL;
}

int Lexer::getNextChar() {
	if(charsDone >= file_length)
		return Lexer::_EOF;
	charsDone++;
	col++;
	//for DOS line endings
	if(file_buffer[charsDone] == '\r' && file_buffer[charsDone+1] == '\n') {
		row++;
		col = 0;
		charsDone++;
		return Lexer::_ENDL;
	}
	//for *nix line endings
	if(file_buffer[charsDone] == '\n') {
		row++;
		col = 0;
		return Lexer::_ENDL;
	}
	
	return file_buffer[charsDone];
}


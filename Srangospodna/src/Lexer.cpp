/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>

#include "Lexer.h"
#include "Constants.h"

Lexer::Lexer(std::string text) {
	col=0;
	row=0;
	this->text = text;
	readedChars=-1;
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;
	
	//read character
	char c = getNextChar();
	
	//some serious stuff
	for(int i = 0; i < CONSTANTS_SEPARATORS_LEN; i++) {
		if(c == *Constants::SEPARATORS[i]) {
			return new Token(c, col, row);
		}
	}
	
    return NULL;
}

Lexer::~Lexer() {
}

char Lexer::getNextChar() {
	int ololo = text.length() - 1;
	
	if(readedChars >= ololo)
		return 0;
	
	char c;
	
	readedChars++;
	col++;
	
	c = text.at(readedChars);
	
	if(c == '\n') {
		row++;
		col = 0;
	}
	
	return c;
}


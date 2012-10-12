/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>

#include "Lexer.h"
#include "Constants.h"
#include "Tag.h"
Lexer::Lexer(std::string text) {
	col=1;
	row=1;
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
	if(c == '"'){
		char pc;
		do{
			pc = c;
			c = getNextChar();
			if(c == '\\'){
				c = getNextChar();
				nts.push_back(c);
				//TODO перенос рядку, символ ", та інші символи, які треба екранувати
				//TODO отримати наступний чар, якщо поточний чар "
				//TODO для того, щоб цикл не завершився
			}
			if(c != '"')
				nts.push_back(c);
		}while(c != '"');
		return new StringToken(Tag::STRING,col,row,nts);
	}
    return NULL;
}

//Lexer::~Lexer() {
//}

char Lexer::getNextChar() {
	int textSize = text.length() - 1;
	
	if(readedChars >= textSize)
		return EOF;
	
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


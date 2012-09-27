/* 
 * File:   Lexer.h
 * Author: CrashTUA
 *
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#ifndef LEXER_H
#define	LEXER_H
#include "Token.h"
#include <string>
class Lexer {
public:
    Lexer(std::string text);
    Token* scan();
    virtual ~Lexer();
private:
	char getNextChar();
	std::string text;
	int col;
	int row;
	int readedChars;
};

#endif	/* LEXER_H */


/* 
 * File:   Lexer.h
 * Author: CrashTUA
 *
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#ifndef LEXER_H
#define	LEXER_H
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include "Token.h"
using namespace std;


namespace lexer {
class Lexer {
public:
	Lexer(string source);
	token::Token* scan();
	~Lexer() {
	}
private:
	bool isValidIdChar(int c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
	}
	int file_length;
	string file_buffer;
	ifstream file_is;
	int col;
	int row;
	int charsDone;
};
}
#endif	/* LEXER_H */


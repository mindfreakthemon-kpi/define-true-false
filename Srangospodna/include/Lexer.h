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
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
namespace lexer {
class Lexer {
public:
	Lexer(std::string file);
  Lexer(char[]);
	token::Token* scan();
	~Lexer() {
		delete file_buffer;
	}
private:
	int file_length;
	char * file_buffer;
	ifstream file_is;
	int col;
	int row;
	int charsDone;
};
}
#endif	/* LEXER_H */


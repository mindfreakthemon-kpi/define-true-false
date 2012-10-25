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
#include <fstream>
#include <vector>
using namespace std;
namespace lexer {
class Lexer {
public:
	Lexer(string source);
	token::Token* scan();
	~Lexer() {}
private:
	int file_length;
	string file_buffer;
	ifstream file_is;
	int col;
	int row;
	int charsDone;
};
}
#endif	/* LEXER_H */


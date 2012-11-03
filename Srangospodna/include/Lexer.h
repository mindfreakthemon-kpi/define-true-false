#ifndef LEXER_H
#define	LEXER_H

#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "Token.h"

namespace lexer {

class Lexer {
public:
	Lexer(std::string source);
	Token* scan();
	~Lexer() {
	}
private:
	bool isValidIdChar(int c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
	}
	int file_length;
	std::string file_buffer;
	int col;
	int row;
	int charsDone;
};

}
#endif	/* LEXER_H */


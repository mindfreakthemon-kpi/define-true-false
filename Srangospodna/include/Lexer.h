#ifndef LEXER_H
#define	LEXER_H

#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "Token.h"
class Lexer {
public:
	Lexer(std::string source);
	Token* scan();
	~Lexer() {
	}
private:
	/**
	 * Look for next char. Source location wouldnt be changed.
	 * @param remSpace if true, will return space char
	 * @returns int value of next char
	 */
	int lookNextChar(bool remSpace = true);
	/**
	 * Get next char. Source location would be changed.
	 * @param remSpace if true, will return space char
	 * @returns int value of next char
	 */
	int getNextChar(bool remSpace = true);
	/**
	 * Check char if it valid identifier char.
	 * @param c char for check
	 * @returns true, if char is valid
	 */
	bool isIdentifierChar(int c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
	}
	int file_length;
	std::string file_buffer;
	int col;
	int row;
	int charsDone;
};
#endif	/* LEXER_H */

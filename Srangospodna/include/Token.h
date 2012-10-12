/* 
 * File:   Token.h
 * Author: mindfreakthemon
 *
 * Created on September 27, 2012, 3:44 PM
 */

#ifndef TOKEN_H
#define	TOKEN_H
#include <string>

namespace token {
	enum TokenKind {
		//one symbol tokens
		LF_PARENTHESES = '(',
		RT_PARENTHESES = ')',
		LF_CR_BRACKET = '{',
		RT_CR_BRACKET = '}',
		LF_SQ_BRACKET = '[',
		RT_SQ_BRACKET = ']',

		SEMICOLON = ';',
		COLON = ':',
		COMMA = ',',
		LESS = '<',
		MORE = '>',
		
		PLUS = '+',
		MINUS = '-',
		MULT = '*',
		DIVIDE = '/',
		CARET = '^',
		
		DOUBLE_QUOTE = '"',
		QUOTE = '\'',

		ID = 256,
		INT = 257,
		DOUBLE = 258,
		STRING = 259,
		BOOL = 260,
	
		OPERATOR = 261,
		COMMENT = 262,
	
		TYPE_SCALAR = 263,
		TYPE_ARRAY = 264
	};
} 


class Token {
public:	
	Token(int type, int col, int row):type(type),col(col),row(row) {}

	//virtual ~Token
	//();
	
	int getKind() {
		return type;
	}
private:
	int type;
	int col;
	int row;
};

class StringToken : public Token {
public:
	StringToken(int type, int col, int row, std::string data):Token(type,col,row),data(data) {}

	std::string getData(){
		return data;
	}
private:
	std::string data;
};

#endif	/* TOKEN_H */


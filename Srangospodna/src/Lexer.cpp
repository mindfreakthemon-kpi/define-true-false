#include "Lexer.h"

#include <stddef.h>
#include <iostream>
#include <sstream>
int strToInt(std::string s)
{
	int res;
	std::stringstream ss;
	ss << s;
	ss >> res;
	return res;
}
double strToDouble(std::string s)
{
	double res;
	std::stringstream ss;
	ss << s;
	ss >> res;
	return res;
}
Lexer::Lexer(std::string source) :
		col(0), row(1), charsDone(-1), file_buffer(source) {
	file_length = source.size();
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;
	int c = getNextChar();
	SourceLocation sl = SourceLocation(row, col);
	switch (c) {
	case -1 :
		return new Token(token::LT, SourceLocation(row, col));
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9': {
		bool isDouble = false;
		nts.push_back(c);
		c = lookNextChar();
		while((c >= '0' && c <= '9') || c == '.')
		{
			c = getNextChar();
			if(c == '.')
				isDouble = true;
			nts.push_back(c);
			c = lookNextChar();
		}
		if(isDouble)
			return new Token(token::DOUBLE_LITERAL, sl, strToDouble(nts));
		else
			return new Token(token::INT_LITERAL, sl, strToInt(nts));
	}
	case '\'':
		return new Token(token::QUOTE,sl);
	case '{':
		return new Token(token::LF_CR_BRACKET, sl);
	case '}':
		return new Token(token::RT_CR_BRACKET, sl);
	case '(':
		return new Token(token::LF_PARENTHESES, sl);
	case ')':
		return new Token(token::RT_PARENTHESES, sl);
	case '[':
		return new Token(token::LF_SQ_BRACKET, sl);
	case ']':
		return new Token(token::RT_SQ_BRACKET, sl);
	case ';':
		return new Token(token::SEMICOLON, sl);
	case ':':
		return new Token(token::COLON, sl);
	case ',':
		return new Token(token::COMMA, sl);
	case '<':
		if (lookNextChar() == '=') {
			getNextChar();
			return new Token(token::LESS_EQUALS, sl);
		} else
			return new Token(token::LESS, sl);
	case '>':
		if (lookNextChar() == '=') {
			getNextChar();
			return new Token(token::MORE_EQUALS, sl);
		} else
			return new Token(token::MORE, sl);
	case '+':
		return new Token(token::PLUS, sl);
	case '-':
		return new Token(token::MINUS, sl);
	case '*':
		return new Token(token::MULT, sl);
	case '/':
		return new Token(token::DIVIDE, sl);
	case '^':
		return new Token(token::CARET, sl);
	case '=':
		if (lookNextChar() == '=') {
			getNextChar();
			return new Token(token::DEQUALS, sl);
		} else
			return new Token(token::EQUALS, sl);
	case '&':
		if (lookNextChar() == '&') {
			getNextChar();
			return new Token(token::DAND, sl);
		} else
			return new Token(token::AND, sl);
	case '|':
		if (lookNextChar() == '|') {
			getNextChar();
			return new Token(token::DOR, sl);
		} else
			return new Token(token::OR, sl);
	case '!':
		if (lookNextChar() == '=') {
			getNextChar();
			return new Token(token::NEQUALS, sl);
		} else
			return new Token(token::NEGATION, sl);
	case '~':
		return new Token(token::TILDE, sl);
		//STRING
	case '"': {
		SourceLocation pos(row, col);
		c = getNextChar(false);
		while (c != '"') {
			nts.push_back(c);
			c = getNextChar(false);
		};
		return new Token(token::STRING_LITERAL, pos, nts);
	}

	default: {
		if(isIdentifierChar(c))
		{
			nts.push_back(c);
			c = lookNextChar(false);
		}
		while (isIdentifierChar(c)) {
			c = getNextChar(false);
			nts.push_back(c);
			c = lookNextChar(false);
		}
		//check if it keyword
		if (nts.compare("function") == 0) {
			return new Token(token::FUNCTION, sl);
		}
		if (nts.compare("var") == 0) {
			return new Token(token::VAR, sl);
		}
		if (nts.compare("if") == 0) {
			return new Token(token::IF, sl);
		}
		if (nts.compare("else") == 0) {
			return new Token(token::ELSE, sl);
		}
		if (nts.compare("while") == 0) {
			return new Token(token::WHILE, sl);
		}
		if (nts.compare("return") == 0) {
			return new Token(token::RETURN, sl);
		}
		//types
		if (nts.compare("int") == 0) {
			return new Token(token::TYPE, sl, token::TYPE_INT);
		}
		if (nts.compare("double") == 0) {
			return new Token(token::TYPE, sl, token::TYPE_DOUBLE);
		}
		if (nts.compare("bool") == 0) {
			return new Token(token::TYPE, sl, token::TYPE_BOOL);
		}
		if (nts.compare("string") == 0) {
			return new Token(token::TYPE, sl, token::TYPE_STRING);
		}
		if (nts.compare("void") == 0) {
			return new Token(token::TYPE, sl, token::TYPE_VOID);
		}
		//for bool literals
		if (nts.compare("true") == 0) {
			return new Token(token::BOOL_LITERAL, sl, true);
		}
		if (nts.compare("false") == 0) {
			return new Token(token::BOOL_LITERAL, sl, false);
		}
		//non keyword(id)
		return new Token(token::ID, sl, nts);
		break;
	}
	}
	return NULL;
}

int Lexer::lookNextChar(bool remSpace) {
	int c;
	bool done = true;
	int localcharsDone = charsDone;
	//read character
	while (done) {
		localcharsDone++;
		if (localcharsDone >= file_length) {
			//end of file
			return -1;
		}
		//for DOS line endings
		if (file_buffer[localcharsDone] == '\r'
				&& file_buffer[localcharsDone + 1] == '\n') {
			localcharsDone = localcharsDone + 2;
		}
		//for *nix line endings
		if (file_buffer[localcharsDone] == '\n') {
			localcharsDone++;
		}
		c = file_buffer[localcharsDone];
		//remove whitespace
		if(remSpace)
		{
			if (c != ' ')
				done = false;
		}
		else
			done = false;

	}
	return c;
}

int Lexer::getNextChar(bool remSpace) {
	int c;
	bool done = true;
	//read character
	while (done) {
		charsDone++;
		col++;
		if (charsDone >= file_length) {
			//end of file
			return -1;
		}
		//for DOS line endings
		if (file_buffer[charsDone] == '\r'
				&& file_buffer[charsDone + 1] == '\n') {
			row++;
			col = 1;
			charsDone = charsDone + 2;
		}
		//for *nix line endings
		if (file_buffer[charsDone] == '\n') {
			row++;
			col = 1;
			charsDone++;
		}
		c = file_buffer[charsDone];
		//remove whitespace
		if(remSpace)
		{
			if (c != ' ')
				done = false;
		}
		else
			done = false;
	}
	return c;
}


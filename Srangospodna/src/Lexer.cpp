#include "Lexer.h"

#include <stddef.h>
#include <iostream>

using namespace lexer;

Lexer::Lexer(std::string source) :
		col(0), row(1), charsDone(-1), file_buffer(source) {
	file_length = source.size();
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;
	bool done = true;
	int c;
	//read character
	while (done) {
		charsDone++;
		col++;
		if (charsDone >= file_length) {
			//end of file
			return new Token(token::LT, SourceLocation(row, col + 1));
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
		if (c != ' ')
			done = false;
	}

	//some serious stuff
	SourceLocation sl = SourceLocation(row, col);
	switch (c) {
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
		int value = 0;
		while ((charsDone < file_length) && file_buffer[charsDone] >= '0'
				&& file_buffer[charsDone] <= '9') {
			value *= 10;
			value += file_buffer[charsDone] - '0';
			charsDone++;
			col++;
		}
		//warning, if no token founded this will cause inf loop
		charsDone--;
		col--;
		return new Token(token::INT, sl, value);
	}
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
		//here goes operators
	case '<':
		if (file_buffer[charsDone + 1] == '=') {
			charsDone++;
			return new Token(token::LESS_EQUALS, sl);
		} else
			return new Token(token::LESS, sl);
	case '>':
		if (file_buffer[charsDone + 1] == '=') {
			charsDone++;
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
		if (file_buffer[charsDone + 1] == '=') {
			charsDone++;
			return new Token(token::DEQUALS, sl);
		} else
			return new Token(token::EQUALS, sl);
	case '&':
		if (file_buffer[charsDone + 1] == '&') {
			charsDone++;
			return new Token(token::DAND, sl);
		} else
			return new Token(token::AND, sl);
	case '|':
		if (file_buffer[charsDone + 1] == '|') {
			charsDone++;
			return new Token(token::DOR, sl);
		} else
			return new Token(token::OR, sl);
	case '!':
		if (file_buffer[charsDone + 1] == '=') {
			charsDone++;
			return new Token(token::NEQUALS, sl);
		} else
			return new Token(token::NEGATION, sl);
	case '~':
		return new Token(token::TILDE, sl);
		//STRING
	case '"': {
		SourceLocation pos(row, col);
		while (file_buffer[++charsDone] != '"') {
			nts.push_back(file_buffer[charsDone]);
		};
		col = col + nts.length();
		return new Token(token::STRING, pos, nts);
	}

	default: {
		while ((charsDone < file_length)
				&& (isalnum(file_buffer[charsDone]) != 0
						|| file_buffer[charsDone] == '_')) {
			nts.push_back(file_buffer[charsDone]);
			charsDone++;
			col++;
		}
		//warning, if no token founded this will cause inf loop
		charsDone--;
		col--;
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
		//types!!!!
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
		//check if int

		// TODO more keywords !!!
		//non keyword(id)
		return new Token(token::ID, sl, nts);
		break;
	}
	}
	return NULL;
}

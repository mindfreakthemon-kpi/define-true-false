#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <cassert>
#include <vector>
#include <cstdint>

#include "Lexer.h"

namespace parser {

class program {
public:
	program(std::vector<funcDecl> funcDeclList):funcDeclList(funcDeclList) {}
	
	std::vector<funcDecl> getFuncDeclList() {
		return funcDeclList;
	}
private:
	std::vector<funcDecl> funcDeclList;
}

class Parser
{
public:
	//no autoconstructing
	explicit parser(std::vector<Token> source) : source(std::move(source)),
		next_index(0), row(1), col(1) {}
		
	program parse() {
		std::vector<funcDecl> r;
		
		return new program(r);
	}

private:
	bool is_eof() const
	{
		return source[next_index] == token::TokenKind::LT;
	}

	token::Token look_token() const
	{
		assert(!is_eof());
		return source[next_index];
	}
	
	void consume_token()
	{
		assert(!is_eof());
		next_index++;
	}

	std::vector<token::Token> source;
	size_t next_index;
	unsigned row;
	unsigned col;
};

}

#endif /* PARSER_H */
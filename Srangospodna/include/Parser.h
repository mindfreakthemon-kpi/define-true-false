#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <cassert>
#include <vector>

#include <ostream>
#include <cstdint>

#include "Token.h"
#include "Node.h"

class Parser {
public:
	//no autoconstructing
	explicit Parser(std::vector<Token> source) : 
		source(std::move(source)),
		next_index(0) {}
		
	node::program * parse();
	node::funcDecl * parseFuncDecl();

private:
 	bool is_eof() const {
		return source[next_index].getKind() == token::LT;
	}

	Token look_token() const {
		return source[next_index];
	}
	
	void consume_token() {
		assert(!is_eof());
		next_index++;
	}

	std::vector<Token> source;
	size_t next_index;
};

#endif /* PARSER_H */

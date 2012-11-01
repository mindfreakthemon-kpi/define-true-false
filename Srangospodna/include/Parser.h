#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <cassert>
#include <vector>

#include <ostream>
#include <cstdint>

#include "Token.h"
#include "Node.h"
#include "Error.h"

class Parser {
public:
	//no autoconstructing
	explicit Parser(std::vector<Token> source, ErrorLoggerWrapper *logger) : 
		source(std::move(source)),
		logger(logger),
		next_index(0) {}
		
	node::program *parse();
	node::funcDecl *parseFuncDecl();
	std::vector<node::varDecl *> *parseFuncParamDeclList();
	node::varType *parseVarType();

private:
 	bool is_eof() {
		return source[next_index].getKind() == token::LT;
	}

	Token look_token() {
		return source[next_index];
	}
	
	void consume_token() {
		assert(!is_eof());
		next_index++;
	}
	
	SourceLocation point_token() {
		return look_token().getLocation();
	}
	
	std::string recognize_token() {
		return look_token().getKindString();
	}
	
	std::vector<Token> source;
	size_t next_index;
	
	ErrorLoggerWrapper *logger;
};

#endif /* PARSER_H */

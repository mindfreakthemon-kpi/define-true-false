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
		
	node::Program *parse();
	
private:
	node::FuncDecl *parseFuncDecl();
	std::vector<node::VarDecl *> *parseFuncParamDeclList();
	std::vector<node::VarDecl *> *parseFuncLocalVarsList();
	node::VarType *parseVarType();
	std::vector<node::Statement *> *parseStatementList();
	node::Statement *parseIfStatement();
	node::Statement *parseWhileStatement();
	node::Statement *parseReturnStatement();
	node::Statement *parseExpressionOrAssignmentStatement();	
	node::BoolExpression *parseBoolExpression();
	std::vector<node::VarDecl *> *parseVarDeclList();
	
	Token next_token() {
		return source[next_index];
	}
	
	token::TokenKind next_kind() {
		return next_token().getKind();
	}
	
	bool is_eof() {
		return next_kind() == token::LT;
	}
	
	void consume_token() {
		assert(!is_eof());
		next_index++;
	}
	
	SourceLocation point_token() {
		return next_token().getLocation();
	}
	
	std::string recognize_token() {
		return next_token().getKindString();
	}
	
	std::vector<Token> source;
	size_t next_index;
	
	ErrorLoggerWrapper *logger;
};

#endif /* PARSER_H */

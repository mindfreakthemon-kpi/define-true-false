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
	Parser(std::vector<Token> source, ErrorLoggerWrapper *logger) : 
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
	std::vector<node::VarDecl *> *parseVarDeclList();
	node::Expression *parseBoolExpression();
	node::Expression *parseBoolTerm();
	node::Expression *parseBoolNotFactor();
	node::Expression *parseBoolRelation();
	node::Expression *parseBinExpression();
	node::Expression *parseBinTerm();
	node::Expression *parseBinNotFactor();
	node::Expression *parseMathExpression();
	node::Expression *parseMathTerm();
	node::Expression *parseMathSignedFactor();
	node::Expression *parseParenthesesExpression();
	node::Expression *parseOperand();	
	node::Expression *parseFuncCallExpression(std::string *name);
	node::Expression *parseArrayAccessExpression(std::string *name);
	
	
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
		return token::getKindString(next_kind());
	}
	
	std::vector<Token> source;
	size_t next_index;
	
	ErrorLoggerWrapper *logger;
};

#endif /* PARSER_H */

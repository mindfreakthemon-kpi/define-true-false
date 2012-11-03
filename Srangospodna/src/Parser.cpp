#include "Parser.h"


#include <cstdio>
#define TRACE_MSG fprintf(stdout, __FUNCTION__,     \
                          "() [%s:%d] here I am\n", \
                          __FILE__, __LINE__)

node::Program *Parser::parse() {
	std::vector<node::FuncDecl *> r;
	node::FuncDecl *fD;
			
	while(next_kind() == token::FUNCTION) {
		fD = parseFuncDecl();
		
		if(fD == NULL)
			return NULL;
		
		r.push_back(fD);
	}
	
	if(!is_eof()) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected FUNCTION");
		return NULL;
	}	
	
	return new node::Program(r);
};

std::vector<node::VarDecl *> *Parser::parseVarDeclList() {
	std::vector<node::VarDecl *> *vDL = new std::vector<node::VarDecl *>;
	std::string name;
	
	while(!is_eof()) {
		if(next_kind() != token::ID) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ID");
			return NULL;
		}
		
		name = next_token().getStringData();
		consume_token();
		
		if(next_kind() != token::COLON) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected COLON");
			return NULL;
		}
		
		consume_token();
		
		if(next_kind() != token::TYPE) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected TYPE");
			return NULL;
		}
		
		node::VarType *vT = parseVarType();
		
		vDL->push_back(new node::VarDecl(name, vT));
		
		if(next_kind() != token::COMMA) {
			break;
		}
		
		consume_token();
	}
	
	return vDL;
};

node::FuncDecl *Parser::parseFuncDecl() {
	assert(next_kind() == token::FUNCTION);
	consume_token();
		
	if(next_kind() != token::ID) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ID");
		return NULL;
	}
	
	Token id = next_token();
	consume_token();
	
	if(next_kind() != token::LF_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	std::vector<node::VarDecl *> *fPDL;
	
	if(next_kind() == token::RT_PARENTHESES) {
		fPDL = new std::vector<node::VarDecl *>;
	} else {
		fPDL = parseVarDeclList();
		
		if(fPDL == NULL)
			return NULL;
			
		if(next_kind() != token::RT_PARENTHESES) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
			return NULL;
		}
	}
	
	// rt parentheses
	consume_token();
	
	if(next_kind() != token::COLON) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected COLON");
		return NULL;
	}
	
	consume_token();	
	node::VarType *rT = parseVarType();
	
	if(rT == NULL)
		return NULL;
	
	if(next_kind() != token::LF_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	std::vector<node::VarDecl *> *fLVL;
	
	if(next_kind() == token::VAR) {
		consume_token();
		fLVL = parseVarDeclList();
		
		if(fLVL == NULL)
			return NULL;
			
		if(next_kind() != token::SEMICOLON) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
			return NULL;
		}
		
		// semicolon included
		consume_token();
	} else {
		fLVL = new std::vector<node::VarDecl *>;
	}
		
	std::vector<node::Statement *> *sL = parseStatementList();
	
	if(next_kind() != token::RT_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
		return NULL;
	}
	
	// rt_cr bracket included
	consume_token();
	
	return new node::FuncDecl(id.getStringData(), *fPDL, rT, *fLVL, *sL);
};

node::VarType *Parser::parseVarType() {
	assert(next_kind() == token::TYPE);
	
	token::DataType type = next_token().getDataType();
	consume_token();
	
	if(next_kind() == token::LF_SQ_BRACKET) {
		consume_token();
		
		if(next_kind() != token::RT_SQ_BRACKET) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_SQ_BRACKET");
			return NULL;
		}
		
		consume_token();
		
		return new node::VarArrayType(type);
	}

	return new node::VarScalarType(type);
};

std::vector<node::Statement *> *Parser::parseStatementList() {
	node::Statement *s;
	std::vector<node::Statement *> *sL = new std::vector<node::Statement *>;

	while(next_kind() != token::RT_CR_BRACKET && !is_eof()) {
		switch(next_kind()) {
			case token::IF: {
				s = parseIfStatement();
				break;
			}
			case token::WHILE: {
				s = parseWhileStatement();
				break;
			}
			case token::RETURN: {
				s = parseReturnStatement();
				break;
			}
			default: {
				s = parseExpressionOrAssignmentStatement();
			}
		}
		
		if(s == NULL)
			return NULL;
			
		sL->push_back(s);
	}	
	
	return sL;	
};

node::Statement *Parser::parseIfStatement() {
	assert(next_kind() == token::IF);
	consume_token();
	
	if(next_kind() != token::LF_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	node::BoolExpression *bE = parseBoolExpression();
	
	if(next_kind() != token::RT_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	
	if(next_kind() != token::LF_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	std::vector<node::Statement *> *sLT = parseStatementList();
	
	if(next_kind() != token::RT_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	
	if(next_kind() == token::ELSE) {
		consume_token();
		
		if(next_kind() != token::LF_CR_BRACKET) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
			return NULL;
		}
		
		consume_token();
		std::vector<node::Statement *> *sLF = parseStatementList();
		
		if(next_kind() != token::RT_CR_BRACKET) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
			return NULL;
		}
		
		consume_token();
		return new node::IfElseStatement(bE, *sLT, *sLF);
	}
	
	return new node::IfStatement(bE, *sLT);
};

node::Statement *Parser::parseWhileStatement() {
	assert(next_kind() == token::WHILE);
	consume_token();
	
	if(next_kind() != token::LF_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	node::BoolExpression *bE = parseBoolExpression();
	
	if(next_kind() != token::RT_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	
	if(next_kind() != token::LF_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	std::vector<node::Statement *> *sL = parseStatementList();
	
	if(next_kind() != token::RT_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
		return NULL;
	}
	
	consume_token();	
	return new node::WhileStatement(bE, *sL);
};

node::Statement *Parser::parseReturnStatement() {
	assert(next_kind() == token::RETURN);
	consume_token();
	
	if(next_kind() == token::SEMICOLON) {
		consume_token();
		return new node::ReturnStatement(NULL);
	}
	
	node::BoolExpression *bE = parseBoolExpression();
	
	if(next_kind() != token::SEMICOLON) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
		return NULL;
	}
	
	consume_token();	
	return new node::ReturnStatement(bE);
};

node::Statement *Parser::parseExpressionOrAssignmentStatement() {
	node::BoolExpression *bE = parseBoolExpression();
	
	if(bE == NULL)
		return NULL;

	if(next_kind() == token::DEQUALS) {
		consume_token();
		node::BoolExpression *bEV = parseBoolExpression();
		
		if(next_kind() != token::SEMICOLON) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
			return NULL;
		}
		
		// semicolon included
		consume_token();
		
		return new node::AssignmentStatement(bE, bEV);
	}
	
	if(next_kind() != token::SEMICOLON) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
		return NULL;
	}
	
	consume_token();	
	return new node::ExpressionStatement(bE);
};

node::BoolExpression *Parser::parseBoolExpression() {	
	return new node::BoolExpression();
};




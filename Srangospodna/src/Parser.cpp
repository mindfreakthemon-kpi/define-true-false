#include "Parser.h"

#include <cstdio>
#define TRACE_MSG fprintf(stdout, "(%s) [%s:%d] here I am\n", \
                          __FUNCTION__, __FILE__, __LINE__)

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
	
	if(sL == NULL)
		return NULL;
	
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
	node::Expression *bE = parseBoolExpression();
	
	if(bE == NULL)
		return NULL;
	
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
		return new node::IfStatement(bE, *sLT, *sLF);
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
	node::Expression *bE = parseBoolExpression();
	
	if(bE == NULL)
		return NULL;
	
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
	
	node::Expression *bE = parseBoolExpression();
	
	if(bE == NULL)
		return NULL;
	
	if(next_kind() != token::SEMICOLON) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
		return NULL;
	}
	
	consume_token();	
	return new node::ReturnStatement(bE);
};

node::Statement *Parser::parseExpressionOrAssignmentStatement() {
	node::Expression *bE = parseBoolExpression();
	
	if(bE == NULL)
		return NULL;

	if(next_kind() == token::DEQUALS) {
		consume_token();
		node::Expression *bEV = parseBoolExpression();
		
		if(bEV == NULL)
			return NULL;
		
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

node::Expression *Parser::parseBoolExpression() {
	node::Expression *leftExpr = parseBoolTerm();
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::DOR) {
		tK = next_kind();
		consume_token();
		rightExpr = parseBoolTerm();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseBoolTerm() {
	node::Expression *leftExpr = parseBoolNotFactor();
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::DAND) {
		tK = next_kind();
		consume_token();
		rightExpr = parseBoolNotFactor();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseBoolNotFactor() {
	bool negation = false;
	
	if(next_kind() == token::NEGATION) {
		consume_token();
		negation = true;
	}
	
	node::Expression *rightExpr = parseBoolRelation();
	
	if(rightExpr == NULL)
		return NULL;
	
	if(negation) {
		rightExpr = new node::Expression(NULL, rightExpr, token::NEGATION);
	}

	return rightExpr;
};

node::Expression *Parser::parseBoolRelation() {
	node::Expression *leftExpr = parseBinExpression();
	
	if(leftExpr == NULL)
		return NULL;
	
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::DEQUALS || next_kind() == token::NEQUALS ||
		next_kind() == token::LESS_EQUALS || next_kind() == token::MORE_EQUALS ||
		next_kind() == token::MORE || next_kind() == token::LESS) {
		tK = next_kind();
		consume_token();
		rightExpr = parseBinExpression();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseBinExpression() {
	node::Expression *leftExpr = parseBinTerm();
	
	if(leftExpr == NULL)
		return NULL;
	
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::OR || next_kind() == token::CARET) {
		tK = next_kind();
		consume_token();
		rightExpr = parseBinTerm();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseBinTerm() {
	node::Expression *leftExpr = parseBinNotFactor();
	
	if(leftExpr == NULL)
		return NULL;
	
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::AND) {
		tK = next_kind();
		consume_token();
		rightExpr = parseBinNotFactor();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseBinNotFactor() {
	bool negation = false;
	
	if(next_kind() == token::TILDE) {
		consume_token();
		negation = true;
	}
	
	node::Expression *rightExpr = parseMathExpression();
	
	if(rightExpr == NULL)
		return NULL;
	
	if(negation) {
		rightExpr = new node::Expression(NULL, rightExpr, token::TILDE);
	}

	return rightExpr;
};

node::Expression *Parser::parseMathExpression() {
	node::Expression *leftExpr = parseMathTerm();
	
	if(leftExpr == NULL)
		return NULL;
	
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::PLUS || next_kind() == token::MINUS) {
		tK = next_kind();
		consume_token();
		rightExpr = parseMathTerm();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseMathTerm() {
	node::Expression *leftExpr = parseMathSignedFactor();
	
	if(leftExpr == NULL)
		return NULL;
	
	node::Expression *rightExpr;
	token::TokenKind tK;
	
	while(next_kind() == token::MULT || next_kind() == token::DIVIDE) {
		tK = next_kind();
		consume_token();
		rightExpr = parseMathSignedFactor();
		
		if(rightExpr == NULL)
			return NULL;
		
		leftExpr = new node::Expression(leftExpr, rightExpr, tK);		
	}
	
	return leftExpr;
};

node::Expression *Parser::parseMathSignedFactor() {
	bool negation = false;
	
	if(next_kind() == token::MINUS) {
		consume_token();
		negation = true;
	}
	
	node::Expression *rightExpr = parseOperand();
	
	if(rightExpr == NULL)
		return NULL;
	
	if(negation) {
		rightExpr = new node::Expression(NULL, rightExpr, token::MINUS);
	}

	return rightExpr;
};

node::Expression *Parser::parseParenthesesExpression() {
	assert(next_kind() == token::LF_PARENTHESES);
	consume_token();
	
	node::Expression *e = parseBoolExpression();
	
	if(e == NULL)
		return NULL;

	if(next_kind() != token::RT_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
		return NULL;
	}
	
	consume_token();	
	return e;
};

node::Expression *Parser::parseOperand() {
	switch(next_kind()) {
		case token::LF_PARENTHESES:
			return parseParenthesesExpression();
		case token::INT:
			consume_token();
			return new node::IntLiteral(next_token().getIntData());
		case token::DOUBLE:
			consume_token();
			return new node::DoubleLiteral(next_token().getDoubleData());
		case token::STRING:
			consume_token();
			return new node::StringLiteral(next_token().getStringData());
		//case token::BOOL:
		//	consume_token();
		//	return new BoolLiteral(next_token().getBoolData());
		case token::ID: {
			std::string name = next_token().getStringData();
			consume_token();
			
			switch(next_kind()) {
				case token::LF_PARENTHESES:
					return parseFuncCallExpression(&name);
				case token::LF_SQ_BRACKET:
					return parseArrayAccessExpression(&name);
				default:
					return new node::VarReferenceExpression(name);
			}
		}
		default:
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES or INT ",
				"or DOUBLE or STRING or BOOL or ID");
			return NULL;
	}
};

node::Expression *Parser::parseFuncCallExpression(std::string *name) {
	assert(next_kind() == token::LF_PARENTHESES);
	consume_token();
	
	std::vector<node::Expression *> *args = new std::vector<node::Expression *>;
	
	if(next_kind() == token::RT_PARENTHESES) {
		consume_token();
		return new node::FuncCallExpression(*name, *args);
	}
	
	node::Expression *e;
	
	while(!is_eof()) {
		e = parseBoolExpression();
		
		if(e == NULL)
			return NULL;
			
		args->push_back(e);
		
		if(next_kind() != token::COMMA) {
			break;
		}
		
		consume_token();
	}
	
	if(next_kind() != token::RT_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
		return NULL;
	}
	
	consume_token();
	return new node::FuncCallExpression(*name, *args);
};

node::Expression *Parser::parseArrayAccessExpression(std::string *name) {
	assert(next_kind() == token::LF_SQ_BRACKET);
	consume_token();
	
	node::Expression *e = parseMathExpression();
	
	if(e == NULL)
		return NULL;
	
	if(next_kind() != token::RT_SQ_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_SQ_BRACKET");
		return NULL;
	}
	
	consume_token();
	return new node::ArrayAccessExpression(*name, e);	
};
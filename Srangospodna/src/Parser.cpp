#include "Parser.h"

node::program *Parser::parse() {
	std::vector<node::funcDecl *> r;
	node::funcDecl *fD;
			
	while(look_token().getKind() == token::FUNCTION) {
		fD = parseFuncDecl();
		
		if(fD == NULL)
			return NULL;
		
		r.push_back(fD);
	}
	
	if(!is_eof()) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected FUNCTION");
		return NULL;
	}	
	
	return new node::program(r);
};

node::funcDecl *Parser::parseFuncDecl() {
	assert(look_token().getKind() == token::FUNCTION);
	consume_token();
		
	if(look_token().getKind() != token::ID) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ID");
		return NULL;
	}
	
	Token id = look_token();
	consume_token();
	
	if(look_token().getKind() != token::LF_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
		return NULL;
	}
	
	consume_token();	
	std::vector<node::varDecl *> *fPDL = parseFuncParamDeclList();
	
	if(fPDL == NULL)
		return NULL;
	
	assert(look_token().getKind() == token::RT_PARENTHESES);
	consume_token();
	
	if(look_token().getKind() != token::COLON) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected COLON");
		return NULL;
	}
	
	consume_token();	
	node::varType *rT = parseVarType();
	
	if(rT == NULL)
		return NULL;
	
	if(look_token().getKind() != token::LF_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	
	//AHTUNG!
	//for test purpose only
	while(look_token().getKind() != token::RT_CR_BRACKET && !is_eof()) {
		consume_token();
	}
	//ENDOF AHTUNG
	
	if(look_token().getKind() != token::RT_CR_BRACKET) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
		return NULL;
	}
	
	consume_token();
	
	return new node::funcDecl(id.getStringData(), *fPDL, rT/*, NULL*/);
};

std::vector<node::varDecl *> *Parser::parseFuncParamDeclList() {
	std::vector<node::varDecl *> *vDL = new std::vector<node::varDecl *>;
	std::string name;

	if(look_token().getKind() == token::RT_PARENTHESES) {
		return vDL;
	}
		
	while(!is_eof()) {
		if(look_token().getKind() != token::ID) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ID");
			return NULL;
		}
		
		name = look_token().getStringData();
		consume_token();
		
		if(look_token().getKind() != token::COLON) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected COLON");
			return NULL;
		}
		
		consume_token();
		
		if(look_token().getKind() != token::TYPE) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected TYPE");
			return NULL;
		}
		
		node::varType *vT = parseVarType();
		
		vDL->push_back(new node::varDecl(name, vT));
		
		if(look_token().getKind() != token::COMMA) {
			break;
		}
		
		consume_token();
	}
	
	if(look_token().getKind() != token::RT_PARENTHESES) {
		logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
		return NULL;
	}

	return vDL;
}

node::varType *Parser::parseVarType() {
	assert(look_token().getKind() == token::TYPE);
	
	token::DataType type = look_token().getDataType();
	consume_token();
	
	if(look_token().getKind() == token::LF_SQ_BRACKET) {
		consume_token();
		
		if(look_token().getKind() != token::RT_SQ_BRACKET) {
			logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_SQ_BRACKET");
			return NULL;
		}
		
		consume_token();
		
		return new node::varArrayType(type);
	}

	return new node::varType(type);
}


#include "Parser.h"

node::program *Parser::parse() {
	std::vector<node::funcDecl> r;
	node::funcDecl *fD;
			
	while(look_token().getKind() == token::FUNCTION) {
		fD = parseFuncDecl();
		
		if(fD == NULL)
			return NULL;
		
		r.push_back(*fD);
	}
	
	if(!is_eof()) {
		// ERROR: Unexpected $(token kind), expected FUNCTION
		return NULL;
	}	
	
	return new node::program(r);
};

node::funcDecl *Parser::parseFuncDecl() {
	assert(look_token().getKind() == token::FUNCTION);
	consume_token();
		
	if(look_token().getKind() != token::ID) {
		// ERROR: Unexpected $(token kind), expected ID
		return NULL;
	}
	
	Token id = look_token();	
	consume_token();
	
	if(look_token().getKind() != token::LF_PARENTHESES) {
		// ERROR: Unexpected $(token kind), expected LF_PARENTHESES
		return NULL;
	}
	
	consume_token();
	node::funcParamDeclList *fPDL = parseFuncParamDeclList();
	// parseFuncParamDecList must return only after finding RT_PARENTHESES
	assert(look_token().getKind() == token::RT_PARENTHESES);
	consume_token();
	
	if(look_token().getKind() != token::COLON) {
		// ERROR: Unexpected $(token kind), expected COLON
		return NULL;
	}
	
	consume_token();	
	node::returnType *rT = parseReturnType();
	
	if(look_token().getKind() != token::LF_CR_BRACKET) {
		// ERROR: Unexpected $(token kind), expected LF_CR_BRACKET
		return NULL;
	}
	
	consume_token();
	node::funcBlock *fB = parseFuncBlock();	
	// parseFuncBlock must return only after finding RT_CR_BRACKET
	assert(look_token().getKind() == token::RT_CR_BRACKET);
	consume_token();
	
	
	return new node::funcDecl(id, fPDL, rT, fB);
};

node::funcParamDeclList *Parser::parseFuncParamDeclList() {
	while(look_token().getKind() != token::RT_PARENTHESES && !is_eof()) {
		consume_token();
	}

	return new node::funcParamDeclList();
}

node::returnType *Parser::parseReturnType() {
	while(look_token().getKind() != token::LF_CR_BRACKET && !is_eof()) {
		consume_token();
	}
	
	return new node::returnType();
}

node::funcBlock *Parser::parseFuncBlock() {
	while(look_token().getKind() != token::RT_CR_BRACKET && !is_eof()) {
		consume_token();
	}

	return new node::funcBlock();
}


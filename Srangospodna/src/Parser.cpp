#include "Parser.h"

node::program* Parser::parse() {
	std::vector<node::funcDecl> r;
	node::funcDecl* fD;
			
	while(look_token().getKind() == token::FUNCTION) {
		fD = parseFuncDecl();
		
		if(fD == NULL)
			return NULL;
		
		r.push_back(*fD);
	}
	
	if(!is_eof()) {
		// ERROR: Unexpected token kind, expected LT
		return NULL;
	}	
	
	return new node::program(r);
};

node::funcDecl* Parser::parseFuncDecl() {
	assert(look_token().getKind() == token::FUNCTION);
	consume_token();
	
	//id
	//arglist
	//stmtlist
	
	if(look_token().getKind() != token::ID) {
		// ERROR: Unexpected token kind, expected ID
		return NULL;
	}
	
	consume_token();
	
	if(look_token().getKind() != token::LF_PARENTHESES) {
		// ERROR: Unexpected token kind, expected LF_PARENTHESES
		return NULL;
	}
	
	consume_token();
	
	//AHTUNG!
	//for testing purpose
	while(look_token().getKind() != token::RT_CR_BRACKET && !is_eof()) {
		consume_token();	
	}
	//<<<<<<<
	
	if(is_eof()) {
		// ERROR: Unexpected end, expected RT_CR_BRACKET
		return NULL;
	}
	
	consume_token();
	
	return new node::funcDecl();
};
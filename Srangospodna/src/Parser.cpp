#include "Parser.h"

namespace parser {

// PROGRAM =>
class program {
public:
	program(std::vector<funcDecl> funcDeclList) :
		funcDeclList(std::move(funcDeclList)) {}
	
	const std::vector<funcDecl> & getFuncDeclList() {
		return funcDeclList;
	}
private:
	std::vector<funcDecl> funcDeclList;
};

program * Parser::parse() {
	std::vector<funcDecl> r;
	
	return new program(r);
};
// <= PROGRAM

class funcDecl {
	funcDecl() {}
};

}
#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <vector>

namespace node {

class funcBlock {
public:
	funcBlock() {}
};

class returnType {
public:
	returnType() {}
};

class funcParamDeclList {
public:
	funcParamDeclList() {}
};

class funcDecl {
public:
	funcDecl(Token id, funcParamDeclList *fPDL, returnType *rT, funcBlock *fB) : id(id),
		fPDL(std::move(fPDL)), rT(std::move(rT)),
		fB(std::move(fB)) {}	
	
	Token getIDToken() const {
		return id;
	}
	
	funcParamDeclList *getFuncParamDeclList() const {
		return fPDL;
	}
	
	returnType *getReturnType() const {
		return rT;
	}
	
	funcBlock *getFuncBlock() const {
		return fB;
	}
private:
	Token id;
	funcParamDeclList *fPDL;
	returnType *rT;
	funcBlock *fB;
};

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


class SyntaxDumper {
public:
	SyntaxDumper() : ss() {}
	
	void dump(program *p) {		
		std::vector<node::funcDecl> funcDeclList = p->getFuncDeclList();
		
		for(std::vector<node::funcDecl>::iterator it = funcDeclList.begin(); it != funcDeclList.end(); ++it) {
		   dump(*it);
		}
	}
	 
	void dump(const funcDecl &fD) {
		ss << "function " << fD.getIDToken().getID() << "(";
		//ss << funcparamdecllist
		ss << "):";
		//ss << return type
		ss << "{";
		//ss << funcblock
		ss << "}";
	}
	
	std::string getResults() {
		return ss.str();
	}
private:
	std::stringstream ss;
};

}

#endif /* NODE_H */

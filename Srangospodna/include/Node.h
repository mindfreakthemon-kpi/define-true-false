#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <vector>

namespace node {

class funcDecl;
class program;

class NodeVisitor {
public:
	virtual void visit(funcDecl* fD) = 0;
	virtual void visit(program* p) = 0;
};

class Node {
public:
	virtual void accept(class NodeVisitor &v) = 0;
};

class funcDecl : public Node {
public:
	funcDecl() {}	
	
	void accept(NodeVisitor &v) {
		v.visit(this);
	}
};

class program : public Node {
public:
	program(std::vector<funcDecl> funcDeclList) :
		funcDeclList(std::move(funcDeclList)) {}
	
	const std::vector<funcDecl> & getFuncDeclList() {
		return funcDeclList;
	}
	
	void accept(NodeVisitor &v) {
		v.visit(this);
	}
private:
	std::vector<funcDecl> funcDeclList;
};





class CoreDumper_NodeVisitor : public NodeVisitor {
public:
	CoreDumper_NodeVisitor() : ss() {}
	
	void visit(program* p) {
		ss << "PROGRAM::" << std::endl;
		
		std::vector<node::funcDecl> fDlist = p->getFuncDeclList();
		
		for(std::vector<node::funcDecl>::iterator it = fDlist.begin(); it != fDlist.end(); ++it) {
		    (*it).accept(*this);
		}
		
		ss << "ENDPROGRAM" << std::endl;
	}
	 
	void visit(funcDecl* fDw) {
		ss << "Got function" << std::endl;d
	}
	
	std::string getResults() {
		return ss.str();
	}
private:
	std::stringstream ss;
};


}

#endif /* NODE_H */
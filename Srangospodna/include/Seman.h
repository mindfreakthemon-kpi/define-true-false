/*
 * Seman.h
 *
 *  Created on: 06.12.2012
 *      Author: Fors
 */

#ifndef SEMAN_H_
#define SEMAN_H_

#include <string>
#include <sstream>
#include <vector>

namespace node {
class Program;
class FuncDecl;
class VarDecl;
class Statement;
class Expression;
}

namespace Seman {

class ASTVisitor {
public:
	virtual void checkProgram(node::Program *p) = 0;
	virtual void checkFuncDecl(node::FuncDecl *fD) = 0;
	virtual void checkVarDecl(node::VarDecl *vD) = 0;
	virtual void checkStatement(node::Statement *s) = 0;
	virtual void checkExpression(node::Expression *e) = 0;
	virtual ~ASTVisitor() = 0;
};

inline ASTVisitor::~ASTVisitor() {
}

class SyntaxDumper: public ASTVisitor {
public:
	SyntaxDumper();
	virtual void checkProgram(node::Program *p);
	virtual void checkFuncDecl(node::FuncDecl *fD);
	virtual void checkVarDecl(node::VarDecl *vD);
	virtual void checkStatement(node::Statement *s);
	virtual void checkExpression(node::Expression *e);
	void checkVarDeclList(const std::vector<node::VarDecl *> &fPDL);
	void checkStatementList(const std::vector<node::Statement *> &sL);
	void checkExpressionList(const std::vector<node::Expression *> &fAL);
	std::string getResults();
private:
	std::stringstream ss;
};

class Sema {
public:
	void checkAll(node::Program *p);
	std::string getSyntaxDumperResults();
private:
	SyntaxDumper *sD;
};
}

#endif /* SEMAN_H_ */

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
class IfStatement;
class WhileStatement;
class ReturnStatement;
class AssignmentStatement;
class ExpressionStatement;
class Expression;
class UnaryExpression;
class BinaryExpression;
class ParenthesesExpression;
class IntLiteral;
class DoubleLiteral;
class StringLiteral;
class BoolLiteral;
class VarReferenceExpression;
class FuncCallExpression;
class ArrayAccessExpression;
}

namespace Seman {

class ASTVisitor {
public:
	ASTVisitor();
	virtual void checkProgram(node::Program *p);
	virtual void checkFuncDecl(node::FuncDecl *fD);
	virtual void checkVarDecl(node::VarDecl *vD);
	virtual void checkIfStatement(node::IfStatement *s);
	virtual void checkWhileStatement(node::WhileStatement *s);
	virtual void checkReturnStatement(node::ReturnStatement *s);
	virtual void checkAssignmentStatement(node::AssignmentStatement *s);
	virtual void checkExpressionStatement(node::ExpressionStatement *s);
	virtual void checkUnaryExpression(node::UnaryExpression *e);
	virtual void checkBinaryExpression(node::BinaryExpression *e);
	virtual void checkParenthesesExpression(node::ParenthesesExpression *e);
	virtual void checkIntLiteral(node::IntLiteral *e);
	virtual void checkDoubleLiteral(node::DoubleLiteral *e);
	virtual void checkStringLiteral(node::StringLiteral *e);
	virtual void checkBoolLiteral(node::BoolLiteral *e);
	virtual void checkVarReferenceExpression(node::VarReferenceExpression *e);
	virtual void checkFuncCallExpression(node::FuncCallExpression *e);
	virtual void checkArrayAccessExpression(node::ArrayAccessExpression *e);
	void checkVarDeclList(const std::vector<node::VarDecl *> &fPDL);
	void checkStatementList(const std::vector<node::Statement *> &sL);
	void checkExpressionList(const std::vector<node::Expression *> &fAL);
	std::string getResults();
	virtual ~ASTVisitor();
private:
	std::stringstream ss;
};

inline ASTVisitor::~ASTVisitor(){
}

class DuplicatesCheck : public ASTVisitor {
public:
	DuplicatesCheck() : ASTVisitor(){}
	virtual void checkProgram(node::Program *p);
	virtual void checkFuncDecl(node::FuncDecl *fD);
	void checkVarDeclList(const std::vector<node::VarDecl *> &fPDL);
private:
	std::stringstream ss;
};

class Sema {
public:
	void checkAll(node::Program *p);
	std::string getSyntaxDumperResults();
private:
	ASTVisitor *sD;
	DuplicatesCheck *dC;
};
}

#endif /* SEMAN_H_ */

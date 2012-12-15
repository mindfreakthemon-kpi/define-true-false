#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cassert>

#include "Token.h"

namespace Seman {
class ASTVisitor;
}

namespace node {

class Expression {
public:
	virtual void accept(Seman::ASTVisitor *v) = 0;

	SourceLocation getSourceLocation() {
		return loc;
	}

	virtual ~Expression() = 0;
private:
	SourceLocation loc;
};

inline Expression::~Expression() {
}

class UnaryExpression: public Expression {
public:
	UnaryExpression(Expression *expr, token::TokenKind operation,
			SourceLocation loc) :
			operation(operation), expr(expr), loc(loc) {
	}

	Expression *getExpression() const {
		return expr;
	}

	token::TokenKind getOperationType() const {
		return operation;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *expr;
	token::TokenKind operation;
	SourceLocation loc;
};

class BinaryExpression: public Expression {
public:
	BinaryExpression(Expression *leftExpr, Expression *rightExpr,
			token::TokenKind operation, SourceLocation loc) :
			operation(operation), leftExpr(leftExpr), rightExpr(rightExpr), loc(
					loc) {
	}

	Expression *getLeftExpression() const {
		return leftExpr;
	}

	Expression *getRightExpression() const {
		return rightExpr;
	}

	token::TokenKind getOperationType() const {
		return operation;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *leftExpr;
	Expression *rightExpr;
	token::TokenKind operation;
	SourceLocation loc;
};

class ParenthesesExpression: public Expression {
public:
	ParenthesesExpression(Expression *expr, SourceLocation loc) :
			expr(expr), loc(loc) {
	}

	Expression *getExpression() const {
		return expr;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *expr;
	SourceLocation loc;
};

class IntLiteral: public Expression {
public:
	IntLiteral(uint32_t value, SourceLocation loc) :
			value(value), loc(loc) {
	}

	uint32_t getValue() const {
		return value;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	uint32_t value;
	SourceLocation loc;
};

class DoubleLiteral: public Expression {
public:
	DoubleLiteral(double value, SourceLocation loc) :
			value(value), loc(loc) {
	}

	double getValue() const {
		return value;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	double value;
	SourceLocation loc;
};

class StringLiteral: public Expression {
public:
	StringLiteral(std::string value, SourceLocation loc) :
			value(std::move(value)), loc(loc) {
	}

	const std::string &getValue() const {
		return value;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	std::string value;
	SourceLocation loc;
};

class BoolLiteral: public Expression {
public:
	BoolLiteral(bool value, SourceLocation loc) :
			value(value), loc(loc) {
	}

	bool getValue() const {
		return value;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	bool value;
	SourceLocation loc;
};

class VarReferenceExpression: public Expression {
public:
	VarReferenceExpression(std::string name, SourceLocation loc) :
			name(std::move(name)), loc(loc) {
	}

	const std::string &getName() const {
		return name;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	std::string name;
	SourceLocation loc;
};

class FuncCallExpression: public Expression {
public:
	FuncCallExpression(std::string name, std::vector<Expression *> args,
			SourceLocation loc) :
			name(std::move(name)), args(std::move(args)), loc(loc) {
	}

	const std::string &getName() const {
		return name;
	}

	const std::vector<Expression *> &getArguments() const {
		return args;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	std::string name;
	std::vector<Expression *> args;
	SourceLocation loc;
};

class ArrayAccessExpression: public Expression {
public:
	ArrayAccessExpression(std::string name, Expression *index,
			SourceLocation loc) :
			name(std::move(name)), index(index), loc(loc) {
	}

	const std::string &getName() const {
		return name;
	}

	Expression *getIndexExpression() const {
		return index;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	std::string name;
	Expression * index;
	SourceLocation loc;
};

class Statement {
public:
	virtual void accept(Seman::ASTVisitor *v) = 0;

	SourceLocation getSourceLocation() {
		return loc;
	}

	virtual ~Statement() {
	}
private:
	SourceLocation loc;
};

class IfStatement: public Statement {
public:
	IfStatement(Expression *condition, std::vector<node::Statement *> sLT,
			std::vector<node::Statement *> sLF, SourceLocation loc) :
			condition(condition), sLT(std::move(sLT)), sLF(std::move(sLF)), elseBody(
					true), loc(loc) {
	}

	IfStatement(Expression *condition, std::vector<node::Statement *> sLT,
			SourceLocation loc) :
			condition(condition), sLT(std::move(sLT)), elseBody(false), loc(loc) {
	}

	bool hasElseBody() const {
		return elseBody;
	}

	Expression *getCondition() const {
		return condition;
	}

	const std::vector<node::Statement *> &getStatementsListTrue() const {
		return sLT;
	}

	const std::vector<node::Statement *> &getStatementsListFalse() const {
		return sLF;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *condition;
	std::vector<node::Statement *> sLT;
	std::vector<node::Statement *> sLF;
	bool elseBody;
	SourceLocation loc;
};

class WhileStatement: public Statement {
public:
	WhileStatement(Expression *condition, std::vector<node::Statement *> sL,
			SourceLocation loc) :
			condition(condition), sL(std::move(sL)), loc(loc) {
	}

	Expression *getCondition() const {
		return condition;
	}

	const std::vector<node::Statement *> &getStatementsList() const {
		return sL;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *condition;
	std::vector<node::Statement *> sL;
	SourceLocation loc;
};

class ReturnStatement: public Statement {
public:
	ReturnStatement(Expression *retExpr, SourceLocation loc) :
			retExpr(retExpr), loc(loc) {
	}

	Expression *getReturnExpression() const {
		return retExpr;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *retExpr;
	SourceLocation loc;
};

class AssignmentStatement: public Statement {
public:
	AssignmentStatement(Expression *leftExpr, Expression *rightExpr,
			SourceLocation loc) :
			leftExpr(leftExpr), rightExpr(rightExpr), loc(loc) {
	}

	Expression *getLeftExpression() const {
		return leftExpr;
	}

	Expression *getRightExpression() const {
		return rightExpr;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *leftExpr;
	Expression *rightExpr;
	SourceLocation loc;
};

class ExpressionStatement: public Statement {
public:
	ExpressionStatement(Expression *expr, SourceLocation loc) :
			expr(expr), loc(loc) {
	}

	Expression *getExpression() const {
		return expr;
	}

	virtual void accept(Seman::ASTVisitor *v);

private:
	Expression *expr;
	SourceLocation loc;
};

class VarType {
public:
	VarType(token::DataType data_type, SourceLocation loc) :
			data_type(data_type), loc(loc) {
	}

	token::DataType getDataType() {
		return data_type;
	}

	SourceLocation getSourceLocation() {
		return loc;
	}

	virtual ~VarType() {
	}
private:
	token::DataType data_type;
	SourceLocation loc;
};

class VarArrayType: public VarType {
public:
	VarArrayType(token::DataType data_type, SourceLocation loc) :
			VarType(data_type, loc) {
	}
};

class VarScalarType: public VarType {
public:
	VarScalarType(token::DataType data_type, SourceLocation loc) :
			VarType(data_type, loc) {
	}
};

class VarDecl {
public:
	VarDecl(std::string name, VarType *vT, SourceLocation loc) :
			name(std::move(name)), vT(vT), loc(loc) {
	}

	const std::string &getName() const {
		return name;
	}

	VarType *getVarType() const {
		return vT;
	}

	void accept(Seman::ASTVisitor *v);

	SourceLocation getSourceLocation() {
		return loc;
	}

private:
	std::string name;
	VarType *vT; // var type
	SourceLocation loc;
};

class FuncDecl {
public:
	FuncDecl(std::string name, std::vector<VarDecl *> fPDL, VarType *rT,
			std::vector<VarDecl *> fLVL, std::vector<Statement *> sL,
			SourceLocation loc) :
			name(std::move(name)), fPDL(std::move(fPDL)), rT(rT), fLVL(
					std::move(fLVL)), sL(std::move(sL)), loc(loc) {
	}

	const std::string & getName() const {
		return name;
	}

	const std::vector<VarDecl *> &getFuncParamsDeclList() const {
		return fPDL;
	}

	VarType *getReturnType() const {
		return rT;
	}

	const std::vector<VarDecl *> &getFuncLocalVarsList() const {
		return fLVL;
	}

	const std::vector<node::Statement *> &getStatementsList() const {
		return sL;
	}

	void accept(Seman::ASTVisitor *v);

	SourceLocation getSourceLocation() {
		return loc;
	}

private:
	std::string name;
	std::vector<VarDecl *> fPDL; // function param declaration list
	VarType *rT; // return type
	std::vector<VarDecl *> fLVL; // function local vars list
	std::vector<node::Statement *> sL; // statements list
	SourceLocation loc;
};

class Program {
public:
	Program(std::vector<FuncDecl *> fDL, SourceLocation loc) :
			fDL(std::move(fDL)), loc(loc) {
	}

	const std::vector<FuncDecl *> &getFuncDeclList() const {
		return fDL;
	}

	void accept(Seman::ASTVisitor *v);

	SourceLocation getSourceLocation() {
		return loc;
	}

private:
	std::vector<FuncDecl*> fDL; // function declaration list
	SourceLocation loc;
};

}

#endif /* NODE_H */

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
	void accept(Seman::ASTVisitor *v);

	virtual ~Expression() = 0;
};

inline Expression::~Expression() {
}

class UnaryExpression: public Expression {
public:
	UnaryExpression(Expression *expr, token::TokenKind operation) :
			operation(operation), expr(expr) {
	}

	Expression *getExpression() const {
		return expr;
	}

	token::TokenKind getOperationType() const {
		return operation;
	}

private:
	Expression *expr;
	token::TokenKind operation;
};

class BinaryExpression: public Expression {
public:
	BinaryExpression(Expression *leftExpr, Expression *rightExpr,
			token::TokenKind operation) :
			operation(operation), leftExpr(leftExpr), rightExpr(rightExpr) {
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

private:
	Expression *leftExpr;
	Expression *rightExpr;
	token::TokenKind operation;
};

class ParenthesesExpression: public Expression {
public:
	ParenthesesExpression(Expression *expr) :
			expr(expr) {
	}

	Expression *getExpression() const {
		return expr;
	}

private:
	Expression *expr;
};

class IntLiteral: public Expression {
public:
	IntLiteral(uint32_t value) :
			value(value) {
	}

	uint32_t getValue() const {
		return value;
	}

private:
	uint32_t value;
};

class DoubleLiteral: public Expression {
public:
	DoubleLiteral(double value) :
			value(value) {
	}

	double getValue() const {
		return value;
	}

private:
	double value;
};

class StringLiteral: public Expression {
public:
	StringLiteral(std::string value) :
			value(std::move(value)) {
	}

	const std::string &getValue() const {
		return value;
	}

private:
	std::string value;
};

class BoolLiteral: public Expression {
public:
	BoolLiteral(bool value) :
			value(value) {
	}

	bool getValue() const {
		return value;
	}

private:
	bool value;
};

class VarReferenceExpression: public Expression {
public:
	VarReferenceExpression(std::string name) :
			name(std::move(name)) {
	}

	const std::string &getName() const {
		return name;
	}

private:
	std::string name;
};

class FuncCallExpression: public Expression {
public:
	FuncCallExpression(std::string name, std::vector<Expression *> args) :
			name(std::move(name)), args(std::move(args)) {
	}

	const std::string &getName() const {
		return name;
	}

	const std::vector<Expression *> &getArguments() const {
		return args;
	}
private:
	std::string name;
	std::vector<Expression *> args;
};

class ArrayAccessExpression: public Expression {
public:
	ArrayAccessExpression(std::string name, Expression *index) :
			name(std::move(name)), index(index) {
	}

	const std::string &getName() const {
		return name;
	}

	Expression *getIndexExpression() const {
		return index;
	}
private:
	std::string name;
	Expression * index;
};

class Statement {
public:
	void accept(Seman::ASTVisitor *v);

	virtual ~Statement() {
	}
};

class IfStatement: public Statement {
public:
	IfStatement(Expression *condition, std::vector<node::Statement *> sLT,
			std::vector<node::Statement *> sLF) :
			condition(condition), sLT(std::move(sLT)), sLF(std::move(sLF)), elseBody(
					true) {
	}

	IfStatement(Expression *condition, std::vector<node::Statement *> sLT) :
			condition(condition), sLT(std::move(sLT)), elseBody(false) {
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

private:
	Expression *condition;
	std::vector<node::Statement *> sLT;
	std::vector<node::Statement *> sLF;
	bool elseBody;
};

class WhileStatement: public Statement {
public:
	WhileStatement(Expression *condition, std::vector<node::Statement *> sL) :
			condition(condition), sL(std::move(sL)) {
	}

	Expression *getCondition() const {
		return condition;
	}

	const std::vector<node::Statement *> &getStatementsList() const {
		return sL;
	}

private:
	Expression *condition;
	std::vector<node::Statement *> sL;
};

class ReturnStatement: public Statement {
public:
	ReturnStatement(Expression *retExpr) :
			retExpr(retExpr) {
	}

	Expression *getReturnExpression() const {
		return retExpr;
	}

private:
	Expression *retExpr;
};

class AssignmentStatement: public Statement {
public:
	AssignmentStatement(Expression *leftExpr, Expression *rightExpr) :
			leftExpr(leftExpr), rightExpr(rightExpr) {
	}

	Expression *getLeftExpression() const {
		return leftExpr;
	}

	Expression *getRightExpression() const {
		return rightExpr;
	}

private:
	Expression *leftExpr;
	Expression *rightExpr;
};

class ExpressionStatement: public Statement {
public:
	ExpressionStatement(Expression *expr) :
			expr(expr) {
	}

	Expression *getExpression() const {
		return expr;
	}

private:
	Expression *expr;
};

class VarType {
public:
	VarType(token::DataType data_type) :
			data_type(data_type) {
	}

	token::DataType getDataType() {
		return data_type;
	}

	virtual ~VarType() {
	}
private:
	token::DataType data_type;
};

class VarArrayType: public VarType {
public:
	VarArrayType(token::DataType data_type) :
			VarType(data_type) {
	}
};

class VarScalarType: public VarType {
public:
	VarScalarType(token::DataType data_type) :
			VarType(data_type) {
	}
};

class VarDecl {
public:
	VarDecl(std::string name, VarType *vT) :
			name(std::move(name)), vT(vT) {
	}

	const std::string &getName() const {
		return name;
	}

	VarType *getVarType() const {
		return vT;
	}

	void accept(Seman::ASTVisitor *v);

private:
	std::string name;
	VarType *vT; // var type
};

class FuncDecl {
public:
	FuncDecl(std::string name, std::vector<VarDecl *> fPDL, VarType *rT,
			std::vector<VarDecl *> fLVL, std::vector<Statement *> sL) :
			name(std::move(name)), fPDL(std::move(fPDL)), rT(rT), fLVL(
					std::move(fLVL)), sL(std::move(sL)) {
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

private:
	std::string name;
	std::vector<VarDecl *> fPDL; // function param declaration list
	VarType *rT; // return type
	std::vector<VarDecl *> fLVL; // function local vars list
	std::vector<node::Statement *> sL; // statements list
};

class Program {
public:
	Program(std::vector<FuncDecl *> fDL) :
			fDL(std::move(fDL)) {
	}

	const std::vector<FuncDecl *> &getFuncDeclList() const {
		return fDL;
	}

	void accept(Seman::ASTVisitor *v);

private:
	std::vector<FuncDecl*> fDL; // function declaration list
};

}

#endif /* NODE_H */

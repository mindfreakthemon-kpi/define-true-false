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
	UnaryExpression(Expression *expr, token::TokenKind operation);
	Expression *getExpression() const;
	token::TokenKind getOperationType() const;

private:
	Expression *expr;
	token::TokenKind operation;
};

class BinaryExpression: public Expression {
public:
	BinaryExpression(Expression *leftExpr, Expression *rightExpr,
			token::TokenKind operation);
	Expression *getLeftExpression() const;
	Expression *getRightExpression() const;
	token::TokenKind getOperationType() const;

private:
	Expression *leftExpr;
	Expression *rightExpr;
	token::TokenKind operation;
};

class ParenthesesExpression: public Expression {
public:
	ParenthesesExpression(Expression *expr);
	Expression *getExpression() const;

private:
	Expression *expr;
};

class IntLiteral: public Expression {
public:
	IntLiteral(uint32_t value);
	uint32_t getValue() const;

private:
	uint32_t value;
};

class DoubleLiteral: public Expression {
public:
	DoubleLiteral(double value);
	double getValue() const;

private:
	double value;
};

class StringLiteral: public Expression {
public:
	StringLiteral(std::string value);
	const std::string &getValue() const;

private:
	std::string value;
};

class BoolLiteral: public Expression {
public:
	BoolLiteral(bool value);
	bool getValue() const;

private:
	bool value;
};

class VarReferenceExpression: public Expression {
public:
	VarReferenceExpression(std::string name);
	const std::string &getName() const;

private:
	std::string name;
};

class FuncCallExpression: public Expression {
public:
	FuncCallExpression(std::string name, std::vector<Expression *> args);
	const std::string &getName() const;
	const std::vector<Expression *> &getArguments() const;

private:
	std::string name;
	std::vector<Expression *> args;
};

class ArrayAccessExpression: public Expression {
public:
	ArrayAccessExpression(std::string name, Expression *index);
	const std::string &getName() const;
	Expression *getIndexExpression() const;

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
			std::vector<node::Statement *> sLF);
	IfStatement(Expression *condition, std::vector<node::Statement *> sLT);
	bool hasElseBody() const;
	Expression *getCondition() const;
	const std::vector<node::Statement *> &getStatementsListTrue() const;
	const std::vector<node::Statement *> &getStatementsListFalse() const;

private:
	Expression *condition;
	std::vector<node::Statement *> sLT;
	std::vector<node::Statement *> sLF;
	bool elseBody;
};

class WhileStatement: public Statement {
public:
	WhileStatement(Expression *condition, std::vector<node::Statement *> sL);
	Expression *getCondition() const;
	const std::vector<node::Statement *> &getStatementsList() const;

private:
	Expression *condition;
	std::vector<node::Statement *> sL;
};

class ReturnStatement: public Statement {
public:
	ReturnStatement(Expression *retExpr);
	Expression *getReturnExpression() const;

private:
	Expression *retExpr;
};

class AssignmentStatement: public Statement {
public:
	AssignmentStatement(Expression *leftExpr, Expression *rightExpr);
	Expression *getLeftExpression() const;
	Expression *getRightExpression() const;

private:
	Expression *leftExpr;
	Expression *rightExpr;
};

class ExpressionStatement: public Statement {
public:
	ExpressionStatement(Expression *expr);
	Expression *getExpression() const;

private:
	Expression *expr;
};

class VarType {
public:
	VarType(token::DataType data_type);
	token::DataType getDataType();
	virtual ~VarType() {
	}

private:
	token::DataType data_type;
};

class VarArrayType: public VarType {
public:
	VarArrayType(token::DataType data_type);
};

class VarScalarType: public VarType {
public:
	VarScalarType(token::DataType data_type);
};

class VarDecl {
public:
	VarDecl(std::string name, VarType *vT);
	const std::string &getName() const;
	VarType *getVarType() const;
	void accept(Seman::ASTVisitor *v);

private:
	std::string name;
	VarType *vT; // var type
};

class FuncDecl {
public:
	FuncDecl(std::string name, std::vector<VarDecl *> fPDL, VarType *rT,
			std::vector<VarDecl *> fLVL, std::vector<Statement *> sL);
	const std::string & getName() const;
	const std::vector<VarDecl *> &getFuncParamsDeclList() const;
	VarType *getReturnType() const;
	const std::vector<VarDecl *> &getFuncLocalVarsList() const;
	const std::vector<node::Statement *> &getStatementsList() const;
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
	Program(std::vector<FuncDecl *> fDL);
	const std::vector<FuncDecl *> &getFuncDeclList() const;
	void accept(Seman::ASTVisitor *v);

private:
	std::vector<FuncDecl*> fDL; // function declaration list
};

}

#endif /* NODE_H */

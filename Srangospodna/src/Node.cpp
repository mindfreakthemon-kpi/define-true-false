/*
 * Node.cpp
 *
 *  Created on: 07.12.2012
 *      Author: Fors
 */

#include <sstream>
#include <vector>

#include "Node.h"
#include "Seman.h"

using namespace node;

void Expression::accept(Seman::ASTVisitor *v) {
	v->checkExpression(this);
}

UnaryExpression::UnaryExpression(Expression *expr, token::TokenKind operation) :
		operation(operation), expr(expr) {
}

Expression* UnaryExpression::getExpression() const {
	return expr;
}

token::TokenKind UnaryExpression::getOperationType() const {
	return operation;
}

BinaryExpression::BinaryExpression(Expression *leftExpr, Expression *rightExpr,
		token::TokenKind operation) :
		operation(operation), leftExpr(leftExpr), rightExpr(rightExpr) {
}

Expression* BinaryExpression::getLeftExpression() const {
	return leftExpr;
}

Expression* BinaryExpression::getRightExpression() const {
	return rightExpr;
}

token::TokenKind BinaryExpression::getOperationType() const {
	return operation;
}

ParenthesesExpression::ParenthesesExpression(Expression *expr) :
		expr(expr) {
}

Expression* ParenthesesExpression::getExpression() const {
	return expr;
}

IntLiteral::IntLiteral(uint32_t value) :
		value(value) {
}

uint32_t IntLiteral::getValue() const {
	return value;
}

DoubleLiteral::DoubleLiteral(double value) :
		value(value) {
}

double DoubleLiteral::getValue() const {
	return value;
}

StringLiteral::StringLiteral(std::string value) :
		value(std::move(value)) {
}

const std::string& StringLiteral::getValue() const {
	return value;
}

BoolLiteral::BoolLiteral(bool value) :
		value(value) {
}

bool BoolLiteral::getValue() const {
	return value;
}

VarReferenceExpression::VarReferenceExpression(std::string name) :
		name(std::move(name)) {
}

const std::string& VarReferenceExpression::getName() const {
	return name;
}

FuncCallExpression::FuncCallExpression(std::string name,
		std::vector<Expression *> args) :
		name(std::move(name)), args(std::move(args)) {
}

const std::string& FuncCallExpression::getName() const {
	return name;
}

const std::vector<Expression *>& FuncCallExpression::getArguments() const {
	return args;
}

ArrayAccessExpression::ArrayAccessExpression(std::string name,
		Expression *index) :
		name(std::move(name)), index(index) {
}

const std::string& ArrayAccessExpression::getName() const {
	return name;
}

Expression* ArrayAccessExpression::getIndexExpression() const {
	return index;
}

void Statement::accept(Seman::ASTVisitor *v) {
	v->checkStatement(this);
}

IfStatement::IfStatement(Expression *condition,
		std::vector<node::Statement *> sLT, std::vector<node::Statement *> sLF) :
		condition(condition), sLT(std::move(sLT)), sLF(std::move(sLF)), elseBody(
				true) {
}

IfStatement::IfStatement(Expression *condition,
		std::vector<node::Statement *> sLT) :
		condition(condition), sLT(std::move(sLT)), elseBody(false) {
}

bool IfStatement::hasElseBody() const {
	return elseBody;
}

Expression* IfStatement::getCondition() const {
	return condition;
}

const std::vector<node::Statement *>& IfStatement::getStatementsListTrue() const {
	return sLT;
}

const std::vector<node::Statement *>& IfStatement::getStatementsListFalse() const {
	return sLF;
}

WhileStatement::WhileStatement(Expression *condition,
		std::vector<node::Statement *> sL) :
		condition(condition), sL(std::move(sL)) {
}

Expression* WhileStatement::getCondition() const {
	return condition;
}

const std::vector<node::Statement *>& WhileStatement::getStatementsList() const {
	return sL;
}

ReturnStatement::ReturnStatement(Expression *retExpr) :
		retExpr(retExpr) {
}

Expression* ReturnStatement::getReturnExpression() const {
	return retExpr;
}

AssignmentStatement::AssignmentStatement(Expression *leftExpr,
		Expression *rightExpr) :
		leftExpr(leftExpr), rightExpr(rightExpr) {
}

Expression* AssignmentStatement::getLeftExpression() const {
	return leftExpr;
}

Expression* AssignmentStatement::getRightExpression() const {
	return rightExpr;
}

ExpressionStatement::ExpressionStatement(Expression *expr) :
		expr(expr) {
}

Expression* ExpressionStatement::getExpression() const {
	return expr;
}

VarType::VarType(token::DataType data_type) :
		data_type(data_type) {
}

token::DataType VarType::getDataType() {
	return data_type;
}

VarArrayType::VarArrayType(token::DataType data_type) :
		VarType(data_type) {
}

VarScalarType::VarScalarType(token::DataType data_type) :
		VarType(data_type) {
}

	VarDecl::VarDecl(std::string name, VarType *vT) :
	name(std::move(name)), vT(vT) {
	}

	const std::string& VarDecl::getName() const {
		return name;
	}

	VarType * VarDecl::getVarType()
	const {
		return vT;
	}

	void VarDecl::accept(Seman::ASTVisitor *v) {
		v->checkVarDecl(this);
	}

	FuncDecl::FuncDecl(std::string name, std::vector<VarDecl *> fPDL, VarType *rT,
			std::vector<VarDecl *> fLVL, std::vector<Statement *> sL) :
	name(std::move(name)), fPDL(std::move(fPDL)), rT(rT), fLVL(
			std::move(fLVL)), sL(std::move(sL)) {
	}

	const std::string& FuncDecl::getName() const {
		return name;
	}

	const std::vector<VarDecl *>& FuncDecl::getFuncParamsDeclList() const {
		return fPDL;
	}

	VarType * FuncDecl::getReturnType()
const {
	return rT;
}

const std::vector<VarDecl *>& FuncDecl::getFuncLocalVarsList() const {
	return fLVL;
}

const std::vector<node::Statement *>& FuncDecl::getStatementsList() const {
	return sL;
}

void FuncDecl::accept(Seman::ASTVisitor *v) {
	v->checkFuncDecl(this);
}

Program::Program(std::vector<FuncDecl *> fDL) :
fDL(std::move(fDL)) {
}

const std::vector<FuncDecl *>& Program::getFuncDeclList() const {
	return fDL;
}

void Program::accept(Seman::ASTVisitor *v) {
	v->checkProgram(this);
}


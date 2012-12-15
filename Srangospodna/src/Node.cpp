/*
 * Node.cpp
 *
 *  Created on: 07.12.2012
 *      Author: Fors
 */

#include "Node.h"
#include "Seman.h"

using namespace node;

void UnaryExpression::accept(Seman::ASTVisitor *v) {
	v->checkUnaryExpression(this);
}

void BinaryExpression::accept(Seman::ASTVisitor *v) {
	v->checkBinaryExpression(this);
}

void ParenthesesExpression::accept(Seman::ASTVisitor *v) {
	v->checkParenthesesExpression(this);
}

void IntLiteral::accept(Seman::ASTVisitor *v) {
	v->checkIntLiteral(this);
}

void DoubleLiteral::accept(Seman::ASTVisitor *v) {
	v->checkDoubleLiteral(this);
}

void StringLiteral::accept(Seman::ASTVisitor *v) {
	v->checkStringLiteral(this);
}

void BoolLiteral::accept(Seman::ASTVisitor *v) {
	v->checkBoolLiteral(this);
}

void VarReferenceExpression::accept(Seman::ASTVisitor *v) {
	v->checkVarReferenceExpression(this);
}

void FuncCallExpression::accept(Seman::ASTVisitor *v) {
	v->checkFuncCallExpression(this);
}

void ArrayAccessExpression::accept(Seman::ASTVisitor *v) {
	v->checkArrayAccessExpression(this);
}

void Statement::accept(Seman::ASTVisitor *v) {
	v->checkStatement(this);
}

void VarDecl::accept(Seman::ASTVisitor *v) {
	v->checkVarDecl(this);
}

void FuncDecl::accept(Seman::ASTVisitor *v) {
	v->checkFuncDecl(this);
}

void Program::accept(Seman::ASTVisitor *v) {
	v->checkProgram(this);
}


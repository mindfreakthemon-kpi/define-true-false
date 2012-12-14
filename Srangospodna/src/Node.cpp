/*
 * Node.cpp
 *
 *  Created on: 07.12.2012
 *      Author: Fors
 */

#include "Node.h"
#include "Seman.h"

using namespace node;

void Expression::accept(Seman::ASTVisitor *v) {
	v->checkExpression(this);
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


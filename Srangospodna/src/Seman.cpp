/*
 * Seman.cpp
 *
 *  Created on: 07.12.2012
 *      Author: Fors
 */

#include "Seman.h"
#include "Node.h"
#include <iostream>

using namespace Seman;

ASTVisitor::ASTVisitor() :
		ss() {
}

void ASTVisitor::checkProgram(node::Program *p) {
	std::vector<node::FuncDecl *> fDL = p->getFuncDeclList();

	for (std::vector<node::FuncDecl *>::iterator it = fDL.begin();
			it != fDL.end(); ++it) {
		checkFuncDecl(*it);
	}
}

void ASTVisitor::checkFuncDecl(node::FuncDecl *fD) {
	ss << "function " << fD->getName();

	ss << "(";
	checkVarDeclList(fD->getFuncParamsDeclList());
	ss << ")";

	ss << " : " << token::dataTypeString(fD->getReturnType()->getDataType());

	ss << " {";
	if (fD->getFuncLocalVarsList().size() > 0) {
		ss << "var ";
		checkVarDeclList(fD->getFuncLocalVarsList());
		ss << ";";
	}

	checkStatementList(fD->getStatementsList());
	ss << "}";
}

void ASTVisitor::checkVarDeclList(const std::vector<node::VarDecl *> &fPDL) {
	for (std::vector<node::VarDecl *>::const_iterator it = fPDL.begin();
			it != fPDL.end(); ++it) {
		checkVarDecl(*it);

		if ((it + 1) != fPDL.end()) {
			ss << ", ";
		}
	}
}

void ASTVisitor::checkVarDecl(node::VarDecl *vD) {
	ss << vD->getName() << " : "
			<< token::dataTypeString(vD->getVarType()->getDataType());
}

void ASTVisitor::checkStatementList(const std::vector<node::Statement *> &sL) {
	for (std::vector<node::Statement *>::const_iterator it = sL.begin();
			it != sL.end(); ++it) {
		sL.at(it - sL.begin())->accept(this);
	}
}

void ASTVisitor::checkIfStatement(node::IfStatement *s) {
	ss << "if(";
	s->getCondition()->accept(this);
	ss << ") {";
	checkStatementList(s->getStatementsListTrue());
	if (s->hasElseBody()) {
		ss << "} else {";
		checkStatementList(s->getStatementsListFalse());
	}
	ss << "}";
}
void ASTVisitor::checkWhileStatement(node::WhileStatement *s) {
	ss << "while(";
	s->getCondition()->accept(this);
	ss << ") {";
	checkStatementList(s->getStatementsList());
	ss << "}";
}
void ASTVisitor::checkReturnStatement(node::ReturnStatement *s) {
	ss << "return ";
	s->getReturnExpression()->accept(this);
	ss << ";";
}
void ASTVisitor::checkAssignmentStatement(node::AssignmentStatement *s) {
	s->getLeftExpression()->accept(this);
	ss << " = ";
	s->getRightExpression()->accept(this);
	ss << ";";
}
void ASTVisitor::checkExpressionStatement(node::ExpressionStatement *s) {
	s->getExpression()->accept(this);
	ss << ";";
}

void ASTVisitor::checkUnaryExpression(node::UnaryExpression *e) {
	node::Expression *nE = e->getExpression();
	ss << token::getSourceString(e->getOperationType());
	nE->accept(this);
}
void ASTVisitor::checkBinaryExpression(node::BinaryExpression *e) {
	node::Expression *eL = e->getLeftExpression();
	node::Expression *eR = e->getRightExpression();

	if (eL != NULL)
		eL->accept(this);

	ss << token::getSourceString(e->getOperationType());
	eR->accept(this);
}
void ASTVisitor::checkParenthesesExpression(node::ParenthesesExpression *e) {
	ss << "(";
	e->getExpression()->accept(this);
	ss << ")";
}
void ASTVisitor::checkIntLiteral(node::IntLiteral *e) {
	ss << e->getValue();
}
void ASTVisitor::checkDoubleLiteral(node::DoubleLiteral *e) {
	ss << e->getValue();
}
void ASTVisitor::checkStringLiteral(node::StringLiteral *e) {
	ss << e->getValue();
}
void ASTVisitor::checkBoolLiteral(node::BoolLiteral *e) {
	ss << e->getValue();
}
void ASTVisitor::checkVarReferenceExpression(node::VarReferenceExpression *e) {
	ss << e->getName();
}
void ASTVisitor::checkFuncCallExpression(node::FuncCallExpression *e) {
	ss << e->getName() << "(";
	checkExpressionList(e->getArguments());
	ss << ")";
}
void ASTVisitor::checkArrayAccessExpression(node::ArrayAccessExpression *e) {
	ss << e->getName() << "[";
	e->getIndexExpression()->accept(this);
	ss << "]";
}

void ASTVisitor::checkExpressionList(
		const std::vector<node::Expression *> &fAL) {
	for (std::vector<node::Expression *>::const_iterator it = fAL.begin();
			it != fAL.end(); ++it) {
		fAL.at(it - fAL.begin())->accept(this);
		if ((it + 1) != fAL.end()) {
			ss << ", ";
		}
	}
}

std::string ASTVisitor::getResults() {
	return ss.str();
}

void DuplicatesCheck::checkProgram(node::Program *p) {
	std::vector<node::FuncDecl *> fDL = p->getFuncDeclList();

	for (std::vector<node::FuncDecl *>::iterator it1 = fDL.begin();
			it1 != fDL.end(); ++it1) {
		checkFuncDecl(*it1);
		std::string fN = (*it1)->getName();
		for (std::vector<node::FuncDecl *>::iterator it2 = it1;
				it2 != fDL.end(); ++it2) {
			if (it1 != it2 && (*it2)->getName() == fN) {
				logger->error((*it2)->getSourceLocation(), "Duplicate function name found: ", fN);
			}
		}
	}
}

void DuplicatesCheck::checkFuncDecl(node::FuncDecl *fD) {
	checkVarDeclList(fD->getFuncParamsDeclList());
	checkVarDeclList(fD->getFuncLocalVarsList());
}

void DuplicatesCheck::checkVarDeclList(
		const std::vector<node::VarDecl *> &fPDL) {
	for (std::vector<node::VarDecl *>::const_iterator it1 = fPDL.begin();
			it1 != fPDL.end(); ++it1) {
		std::string vN = (*it1)->getName();
		for (std::vector<node::VarDecl *>::const_iterator it2 = it1;
				it2 != fPDL.end(); ++it2) {
			if (it1 != it2 && (*it2)->getName() == vN) {
				logger->error((*it2)->getSourceLocation(), "Duplicate variable name found: ", vN);
			}
		}
	}
}


void ReturnCheck::checkFuncDecl(node::FuncDecl *fD) {
	returnType = fD->getReturnType();
	checkStatementList(fD->getStatementsList());
}

void ReturnCheck::checkStatementList(const std::vector<node::Statement *> &sL) {
	std::vector<node::Statement *>::const_iterator it = sL.begin();
	for (std::vector<node::Statement *>::const_iterator it = sL.begin();
			it != sL.end(); ++it) {
		if (node::ReturnStatement *rS =
				dynamic_cast<node::ReturnStatement *>(*it)) {
			rS->getReturnExpression()->accept(this);
			return;
		}
	}
	logger->error((returnType)->getSourceLocation(), "No return expression");
}

void ReturnCheck::checkUnaryExpression(node::UnaryExpression *e) {

}
void ReturnCheck::checkBinaryExpression(node::BinaryExpression *e) {

}
void ReturnCheck::checkParenthesesExpression(node::ParenthesesExpression *e) {

}
void ReturnCheck::checkIntLiteral(node::IntLiteral *e) {
	if (returnType->getDataType() != token::TYPE_INT) {
		logger->error((*e).getSourceLocation(), "Return type is not ", token::dataTypeString(returnType->getDataType()));
	}
}
void ReturnCheck::checkDoubleLiteral(node::DoubleLiteral *e) {
	if (returnType->getDataType() != token::TYPE_DOUBLE) {
		logger->error((*e).getSourceLocation(), "Return type is not ", token::dataTypeString(returnType->getDataType()));
	}
}
void ReturnCheck::checkStringLiteral(node::StringLiteral *e) {
	if (returnType->getDataType() != token::TYPE_STRING) {
		logger->error((*e).getSourceLocation(), "Return type is not ", token::dataTypeString(returnType->getDataType()));
	}
}
void ReturnCheck::checkBoolLiteral(node::BoolLiteral *e) {
	if (returnType->getDataType() != token::TYPE_BOOL) {
		logger->error((*e).getSourceLocation(), "Return type is not ", token::dataTypeString(returnType->getDataType()));
	}
}
void ReturnCheck::checkVarReferenceExpression(node::VarReferenceExpression *e) {

}
void ReturnCheck::checkFuncCallExpression(node::FuncCallExpression *e) {

}
void ReturnCheck::checkArrayAccessExpression(node::ArrayAccessExpression *e) {

}

void Sema::checkAll(node::Program *p) {
	sD = new ASTVisitor();
	dC = new DuplicatesCheck();
	rC = new ReturnCheck();
	p->accept(sD);
	p->accept(dC);
	p->accept(rC);
}

std::string Sema::getSyntaxDumperResults() {
	return sD->getResults();
}


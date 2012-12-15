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
		checkStatement(*it);
	}
}

void ASTVisitor::checkStatement(node::Statement *s) {
	if (node::IfStatement *iS = dynamic_cast<node::IfStatement *>(s)) {
		ss << "if(";
		checkExpression(iS->getCondition());
		ss << ") {";
		checkStatementList(iS->getStatementsListTrue());
		if (iS->hasElseBody()) {
			ss << "} else {";
			checkStatementList(iS->getStatementsListFalse());
		}
		ss << "}";
	} else if (node::WhileStatement *wS =
			dynamic_cast<node::WhileStatement *>(s)) {
		ss << "while(";
		checkExpression(wS->getCondition());
		ss << ") {";
		checkStatementList(wS->getStatementsList());
		ss << "}";
	} else if (node::ReturnStatement *rS =
			dynamic_cast<node::ReturnStatement *>(s)) {
		ss << "return ";
		checkExpression(rS->getReturnExpression());
		ss << ";";
	} else if (node::AssignmentStatement *aS =
			dynamic_cast<node::AssignmentStatement *>(s)) {
		checkExpression(aS->getLeftExpression());
		ss << " = ";
		checkExpression(aS->getRightExpression());
		ss << ";";
	} else if (node::ExpressionStatement *eS =
			dynamic_cast<node::ExpressionStatement *>(s)) {
		checkExpression(eS->getExpression());
		ss << ";";
	}
}

void ASTVisitor::checkExpression(node::Expression *e) {
	if (node::IntLiteral *iL = dynamic_cast<node::IntLiteral *>(e)) {
		ss << iL->getValue();
	} else if (node::DoubleLiteral *dL = dynamic_cast<node::DoubleLiteral *>(e)) {
		ss << dL->getValue();
	} else if (node::StringLiteral *sL = dynamic_cast<node::StringLiteral *>(e)) {
		ss << sL->getValue();
	} else if (node::BoolLiteral *bL = dynamic_cast<node::BoolLiteral *>(e)) {
		ss << bL->getValue();
	} else if (node::VarReferenceExpression *vRE =
			dynamic_cast<node::VarReferenceExpression *>(e)) {
		ss << vRE->getName();
	} else if (node::FuncCallExpression *fCE =
			dynamic_cast<node::FuncCallExpression *>(e)) {
		ss << fCE->getName() << "(";
		checkExpressionList(fCE->getArguments());
		ss << ")";
	} else if (node::ArrayAccessExpression *aAE =
			dynamic_cast<node::ArrayAccessExpression *>(e)) {
		ss << aAE->getName() << "[";
		checkExpression(aAE->getIndexExpression());
		ss << "]";
	} else if (node::UnaryExpression *uE =
			dynamic_cast<node::UnaryExpression *>(e)) {
		node::Expression *e = uE->getExpression();

		ss << token::getSourceString(uE->getOperationType());
		checkExpression(e);
	} else if (node::BinaryExpression *bE =
			dynamic_cast<node::BinaryExpression *>(e)) {
		node::Expression *eL = bE->getLeftExpression();
		node::Expression *eR = bE->getRightExpression();

		if (eL != NULL)
			checkExpression(eL);

		ss << token::getSourceString(bE->getOperationType());
		checkExpression(eR);
	} else if (node::ParenthesesExpression *pE =
			dynamic_cast<node::ParenthesesExpression *>(e)) {
		ss << "(";
		checkExpression(pE->getExpression());
		ss << ")";
	} else {
		assert(false);
	}
}

void ASTVisitor::checkExpressionList(
		const std::vector<node::Expression *> &fAL) {
	for (std::vector<node::Expression *>::const_iterator it = fAL.begin();
			it != fAL.end(); ++it) {
		checkExpression(*it);

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
		std::string fN = dynamic_cast<node::FuncDecl *>(*it1)->getName();
		for (std::vector<node::FuncDecl *>::iterator it2 = fDL.begin();
				it2 != fDL.end(); ++it2) {
			if (it1 != it2
					&& fN.compare(
							dynamic_cast<node::FuncDecl *>(*it2)->getName())
							== 0) {
				std::cout << "Duplicate function  name found: " << fN << std::endl;
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
		std::string vN = dynamic_cast<node::VarDecl *>(*it1)->getName();
		for (std::vector<node::VarDecl *>::const_iterator it2 = fPDL.begin();
				it2 != fPDL.end(); ++it2) {
			if (it1 != it2
					&& vN.compare(
							dynamic_cast<node::VarDecl *>(*it2)->getName())
							== 0) {
				std::cout << "Duplicate variable name found: " << vN << std::endl;
			}
		}
	}
}

void Sema::checkAll(node::Program *p) {
	sD = new ASTVisitor();
	dC = new DuplicatesCheck();
	p->accept(sD);
	p->accept(dC);
}

std::string Sema::getSyntaxDumperResults() {
	return sD->getResults();
}


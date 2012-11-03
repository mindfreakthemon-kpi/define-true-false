#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <vector>

namespace node {

class Expression {
public:	
	Expression() {}
	
	Expression(Expression *leftExpr, Expression *rightExpr, token::TokenKind operation) :
		operation(operation),
		leftExpr(leftExpr),
		rightExpr(rightExpr) {}
	
	Expression *getLeftSide() const {
		return leftExpr;
	}
	
	Expression *getRightSide() const {
		return rightExpr;
	}
	
	token::TokenKind getOperationType() const {
		return operation;
	}
		
	virtual ~Expression() {}

private:
	Expression *leftExpr;
	Expression *rightExpr;
	
	token::TokenKind operation;
};

class IntLiteral : public Expression {
public:
	IntLiteral(uint32_t value) :
		value(value) {}
		
	uint32_t getValue() const {
		return value;
	}

private:
	uint32_t value;
};

class DoubleLiteral : public Expression {
public:
	DoubleLiteral(double value) :
		value(value) {}
		
	double getValue() const {
		return value;
	}

private:
	double value;
};

class StringLiteral : public Expression {
public:
	StringLiteral(std::string value) :
  		value(std::move(value)) {}
  		
  	const std::string &getValue() const {
  		return value;
  	}

private:
	std::string value;
};

class BoolLiteral : public Expression {
public:
	BoolLiteral(bool value) :
 		value(value) {}
 		
 	bool getValue() const {
 		return value;
 	}

private:
	bool value;
};

class VarReferenceExpression : public Expression {
public:
	VarReferenceExpression(std::string name) : 
		name(std::move(name)) {}
		
	const std::string &getName() const {
		return name;
	}
	
private:
	std::string name;
};

class FuncCallExpression : public Expression {
public:
	FuncCallExpression(std::string name, std::vector<Expression *> args) : 
		name(std::move(name)),
		args(std::move(args)) {}
		
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

class ArrayAccessExpression : public Expression {
public:
	ArrayAccessExpression(std::string name, Expression *index) : 
		name(std::move(name)),
		index(index) {}
		
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
	virtual ~Statement() {}
};

class IfStatement : public Statement {
public:
	IfStatement(Expression *condition, std::vector<node::Statement *> sLT, std::vector<node::Statement *> sLF) :
		condition(condition),
		sLT(std::move(sLT)),
		sLF(std::move(sLF)),
		elseBody(true) {}
		
	IfStatement(Expression *condition, std::vector<node::Statement *> sLT) :
		condition(condition),
		sLT(std::move(sLT)),
		elseBody(false) {}
		
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

class WhileStatement : public Statement {
public:
	WhileStatement(Expression *condition, std::vector<node::Statement *> sL) :
		condition(condition),
		sL(std::move(sL)) {}
	
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

class ReturnStatement : public Statement {
public:
	ReturnStatement(Expression *retExpr) :
		retExpr(retExpr) {}
	
	Expression *getReturnExpression() const {
		return retExpr;
	}
	
private:
	Expression *retExpr;
};

class AssignmentStatement : public Statement {
public:
	AssignmentStatement(Expression *leftExpr, Expression *rightExpr) :
		leftExpr(leftExpr),
		rightExpr(rightExpr) {}
		
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

class ExpressionStatement : public Statement {
public:
	ExpressionStatement(Expression *expr) :
		expr(expr) {}
		
	Expression *getExpression() const {
		return expr;
	}

private:
	Expression *expr;
};

class VarType {
public:
	VarType(token::DataType data_type) :
		data_type(data_type) {}
	
	token::DataType getDataType() {
		return data_type;
	}
	
	virtual ~VarType() {}
private:
	token::DataType data_type;
};

class VarArrayType : public VarType {
public:
	VarArrayType(token::DataType data_type) :
		VarType(data_type) {}
};

class VarScalarType : public VarType {
public:
	VarScalarType(token::DataType data_type) :
		VarType(data_type) {}
};

class VarDecl {
public:
	VarDecl(std::string name, VarType *vT) :
		name(std::move(name)),
		vT(vT) {}
		
	const std::string &getName() const {
		return name;
	} 
	
	VarType *getVarType() const {
		return vT;
	}
	
private:
	std::string name;
	VarType *vT; // var type
};

class FuncDecl {
public:
	FuncDecl(std::string name, std::vector<VarDecl *> fPDL, VarType *rT, std::vector<VarDecl *> fLVL, std::vector<Statement *> sL) :
		name(std::move(name)),
		fPDL(std::move(fPDL)),
		rT(rT),
		fLVL(std::move(fLVL)),
		sL(std::move(sL)) {}	
	
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
		fDL(std::move(fDL)) {}
	
	const std::vector<FuncDecl *> &getFuncDeclList() const {
		return fDL;
	}
	
private:
	std::vector<FuncDecl*> fDL; // function declaration list
};

class SyntaxDumper {
public:
	SyntaxDumper() : ss() {}
	
	void dump(Program *p) {		
		std::vector<FuncDecl *> fDL = p->getFuncDeclList();
		
		for(std::vector<FuncDecl *>::iterator it = fDL.begin(); it != fDL.end(); ++it) {
		   dump(*it);
		}
	}
	 
	void dump(FuncDecl *fD) {
		ss << "function " << fD->getName();
		
		ss << "(";
		dump(fD->getFuncParamsDeclList());	
		ss << ")";
		
		ss << " : " << token::dataTypeString(fD->getReturnType()->getDataType());
		
		ss << " {\n";		
		if(fD->getFuncLocalVarsList().size() > 0) {
			ss << "var ";
			dump(fD->getFuncLocalVarsList());
			ss << ";\n";
		}
		
		dump(fD->getStatementsList());
		ss << "}";
	}
	
	void dump(const std::vector<VarDecl *> &fPDL) {
		for(std::vector<VarDecl *>::const_iterator it = fPDL.begin(); it != fPDL.end(); ++it) {
			dump(*it);
			
			if((it + 1) != fPDL.end()) {
				ss << ", ";
			}
		}
	}
	
	void dump(VarDecl *vD) {
		ss << vD->getName() << " : " << token::dataTypeString(vD->getVarType()->getDataType());
	}
	
	void dump(const std::vector<Statement *> &sL) {
		for(std::vector<Statement *>::const_iterator it = sL.begin(); it != sL.end(); ++it) {
			dump(*it);
		}
	}
	
	void dump(Statement *s) {
		if(IfStatement *iS = dynamic_cast<IfStatement *>(s)) {
			ss << "if(";
			dump(iS->getCondition());
			ss << ") {\n";
			dump(iS->getStatementsListTrue());			
			if(iS->hasElseBody()) {
				ss << "} else {\n";
				dump(iS->getStatementsListFalse());
			}					
			ss << "}\n";
		} else if(WhileStatement *wS = dynamic_cast<WhileStatement *>(s)) {
			ss << "while(";
			dump(wS->getCondition());
			ss << ") {\n";
			dump(wS->getStatementsList());	
			ss << "}\n";
		} else if(ReturnStatement *rS = dynamic_cast<ReturnStatement *>(s)) {
			ss << "return ";
			dump(rS->getReturnExpression());
			ss << ";\n";
		} else if(AssignmentStatement *aS = dynamic_cast<AssignmentStatement *>(s)) {
			dump(aS->getLeftExpression());
			ss << " = ";
			dump(aS->getRightExpression());
			ss << ";\n";
		} else if(ExpressionStatement *eS = dynamic_cast<ExpressionStatement *>(s)) {
			dump(eS->getExpression());
			ss << ";\n";
		}
	}
	
	void dump(Expression *e) {
		if(IntLiteral *iL = dynamic_cast<IntLiteral *>(e)) {
			ss << iL->getValue();
		} else if(DoubleLiteral *dL = dynamic_cast<DoubleLiteral *>(e)) {
			ss << dL->getValue();
		} else if(StringLiteral *sL = dynamic_cast<StringLiteral *>(e)) {
			ss << sL->getValue();
		} else if(BoolLiteral *bL = dynamic_cast<BoolLiteral *>(e)) {
			ss << bL->getValue();
		} else if(VarReferenceExpression *vRE = dynamic_cast<VarReferenceExpression *>(e)) {
			ss << vRE->getName();
		} else if(FuncCallExpression *fCE = dynamic_cast<FuncCallExpression *>(e)) {
			ss << fCE->getName() << "(";
			dump(fCE->getArguments());
			ss << ")";			
		} else if(ArrayAccessExpression *aAE = dynamic_cast<ArrayAccessExpression *>(e)) {
			ss << aAE->getName() << "[";
			dump(aAE->getIndexExpression());
			ss << "]";
		} else {
			Expression *eL = e->getLeftSide();
			Expression *eR = e->getRightSide();
			
			if(eL != NULL)
				dump(eL);
				
			ss << token::getSourceString(e->getOperationType());
			dump(eR);			
		}
	}
	
	void dump(const std::vector<Expression *> &fAL) {
		for(std::vector<Expression *>::const_iterator it = fAL.begin(); it != fAL.end(); ++it) {
			dump(*it);
			
			if((it + 1) != fAL.end()) {
				ss << ", ";
			}
		}
	}
	
	std::string getResults() {
		return ss.str();
	}
	
private:
	std::stringstream ss;
};

}

#endif /* NODE_H */

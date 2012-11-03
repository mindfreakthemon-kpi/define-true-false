#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <vector>

namespace node {

class BoolExpression {
public:
	BoolExpression() {}
};

class Statement {
public:
	virtual ~Statement() {}
};

class IfStatement : public Statement {
public:
	IfStatement(node::BoolExpression *bE, std::vector<node::Statement *> sLT) :
		bE(bE),
		sLT(std::move(sLT)) {}
		
	node::BoolExpression *getBoolExpression() const {
		return bE;
	}
	
	const std::vector<node::Statement *> &getStatementsListTrue() {
		return sLT;
	}
	
private:
	node::BoolExpression *bE;
	std::vector<node::Statement *> sLT;
};

class IfElseStatement : public IfStatement {
public:
	IfElseStatement(node::BoolExpression *bE, std::vector<node::Statement *> sLT, std::vector<node::Statement *> sLF) :
		IfStatement(bE, std::move(sLT)),
		sLF(std::move(sLF)) {}
	
	const std::vector<node::Statement *> &getStatementsListFalse() {
		return sLF;
	}
	
private:
	std::vector<node::Statement *> sLF;
};

class WhileStatement : public Statement {
public:
	WhileStatement(node::BoolExpression *bE, std::vector<node::Statement *> sL) :
		bE(bE),
		sL(std::move(sL)) {}
	
	node::BoolExpression *getBoolExpression() const {
		return bE;
	}
	
	const std::vector<node::Statement *> &getStatementsList() {
		return sL;
	}
	
private:
	node::BoolExpression *bE;
	std::vector<node::Statement *> sL;
};

class ReturnStatement : public Statement {
public:
	ReturnStatement(node::BoolExpression *bE) :
		bE(bE) {}
	
	node::BoolExpression *getBoolExpression() const {
		return bE;
	}
	
private:
	node::BoolExpression *bE;
};

class AssignmentStatement : public Statement {
public:
	AssignmentStatement(node::BoolExpression *bEL, node::BoolExpression *bER) :
		bEL(bEL),
		bER(bER) {}
		
	node::BoolExpression *getBoolExpressionLeft() {
		return bEL;
	}
	
	node::BoolExpression *getBoolExpressionRight() {
		return bER;
	}
	
private:
	node::BoolExpression *bEL;
	node::BoolExpression *bER;
};

class ExpressionStatement : public Statement {
public:
	ExpressionStatement(node::BoolExpression *bE) :
		bE(bE) {}
		
	node::BoolExpression *getBoolExpression() {
		return bE;
	}

private:
	node::BoolExpression *bE;
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
		
	const std::string &getName() {
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
	
	const std::vector<node::Statement *> &getStatementsList() {
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
	
	const std::vector<FuncDecl *> &getFuncDeclList() {
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
		
		ss << " : " << dataTypeToString(fD->getReturnType()->getDataType());
		
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
		ss << vD->getName() << " : " << dataTypeToString(vD->getVarType()->getDataType());
	}
	
	void dump(const std::vector<Statement *> &sL) {
		for(std::vector<Statement *>::const_iterator it = sL.begin(); it != sL.end(); ++it) {
			dump(*it);
		}
	}
	
	void dump(Statement *s) {
		if(IfElseStatement *iES = dynamic_cast<IfElseStatement *>(s)) {
			ss << "if(";
			
			ss << ") {\n";
			
			ss << "} else {\n";
			
			ss << "}\n";
		} else if(IfStatement *iS = dynamic_cast<IfStatement *>(s)) {
			ss << "if(";
			
			ss << ") {\n";
						
			ss << "}\n";
		} else if(WhileStatement *iS = dynamic_cast<WhileStatement *>(s)) {
			ss << "while(";
			
			ss << ") {\n";
						
			ss << "}\n";
		} else if(ReturnStatement *rS = dynamic_cast<ReturnStatement *>(s)) {
		
		} else if(AssignmentStatement *rS = dynamic_cast<AssignmentStatement *>(s)) {
		
		} else if(ExpressionStatement *rS = dynamic_cast<ExpressionStatement *>(s)) {
		
		}
	}
	
	std::string getResults() {
		return ss.str();
	}
	
private:
	std::stringstream ss;
	
	std::string dataTypeToString(token::DataType dT) {
		switch(dT) {
			case token::TYPE_INT: return "int";
			case token::TYPE_DOUBLE: return "double";
			case token::TYPE_STRING: return "string";
			case token::TYPE_BOOL: return "bool";
			case token::TYPE_VOID: return "void";
		}
		
		return "unknown";
	}
};

}

#endif /* NODE_H */

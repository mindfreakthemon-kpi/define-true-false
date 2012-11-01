#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <vector>
#include <unordered_map>

namespace node {

class varType {
public:
	varType(token::DataType data_type) :
		data_type(data_type) {}
	
	token::DataType getDataType() {
		return data_type;
	}
	
	virtual ~varType() {}
private:
	token::DataType data_type;
};

class varArrayType : public varType {
public:
	varArrayType(token::DataType data_type) :
		varType(data_type) {}
};

class varDecl {
public:
	varDecl(std::string name, varType *vT) :
		name(std::move(name)),
		vT(vT) {}
		
	const std::string & getName() {
		return name;
	} 
	
	varType *getVarType() const {
		return vT;
	}
	
private:
	std::string name;
	varType *vT; // var type
};

class funcDecl {
public:
	funcDecl(std::string name, std::vector<varDecl *> fPDL, varType *rT/*@TODO: , std::vector<varDecl *> fLVL, statements*/) :
		name(std::move(name)),
		fPDL(std::move(fPDL)),
		rT(rT)/*,
		fLVL(fLVL)*/ {}	
	
	const std::string & getName() const {
		return name;
	}
	
	const std::vector<varDecl *> & getFuncParamDeclList() const {
		return fPDL;
	}
	
	varType *getReturnType() const {
		return rT;
	}
	
	//const std::vector<varDecl *> & getFuncBlock() const {
	//	return fLVL;
	//}
	
private:
	std::string name;
	std::vector<varDecl *> fPDL; // function param declaration list
	varType *rT; // return type
	//std::vector<varDecl *> fLVL; // function local vars list
};

class program {
public:
	program(std::vector<funcDecl *> fDL) :
		fDL(std::move(fDL)) {}
	
	const std::vector<funcDecl *> & getFuncDeclList() {
		return fDL;
	}
	
private:
	std::vector<funcDecl*> fDL; // function declaration list
};

class SyntaxDumper {
public:
	SyntaxDumper() : ss() {}
	
	void dump(program *p) {		
		std::vector<funcDecl *> fDL = p->getFuncDeclList();
		
		for(std::vector<funcDecl *>::iterator it = fDL.begin(); it != fDL.end(); ++it) {
		   dump(*it);
		}
	}
	 
	void dump(funcDecl *fD) {
		ss << "function " << fD->getName() << "(";
		
		dump(fD->getFuncParamDeclList());	
			
		ss << ") : ";
		
		ss << dataTypeToString(fD->getReturnType()->getDataType());
		
		ss << " {";
		//ss << funcblock
		ss << "}";
	}
	
	void dump(const std::vector<varDecl *> & fPDL) {
		for(std::vector<varDecl *>::const_iterator it = fPDL.begin(); it != fPDL.end(); ++it) {
		   dump(*it);
		   
		   if((it + 1) != fPDL.end()) {
		   	ss << ", ";
		   }
		}
	}
	
	void dump(varDecl *vD) {
		ss << vD->getName() << " : " << dataTypeToString(vD->getVarType()->getDataType());
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

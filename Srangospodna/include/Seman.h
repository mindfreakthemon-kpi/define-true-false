/*
 * Seman.h
 *
 *  Created on: 06.12.2012
 *      Author: Fors
 */

#ifndef SEMAN_H_
#define SEMAN_H_

namespace node {
class Program;
class FuncDecl;
class VarDecl;
class Statement;
class Expression;
}

namespace Seman {

class Seman {
public:
	virtual void checkProgram(node::Program *p);
	virtual void checkFuncDecl(node::FuncDecl *fD);
	virtual void checkVarDecl(node::VarDecl *vD);
	virtual void checkStatement(node::Statement *s);
	virtual void checkExpression(node::Expression *e);
	virtual ~Seman();
};

}

#endif /* SEMAN_H_ */

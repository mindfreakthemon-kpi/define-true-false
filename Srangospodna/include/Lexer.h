/* 
 * File:   Lexer.h
 * Author: CrashTUA
 *
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#ifndef LEXER_H
#define	LEXER_H
#include "Token.h"
class Lexer {
public:
    Lexer();
    Lexer(const Lexer& orig);
    Token* scan();
    virtual ~Lexer();
private:

};

#endif	/* LEXER_H */


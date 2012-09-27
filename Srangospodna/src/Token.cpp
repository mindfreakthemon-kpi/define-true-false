/* 
 * File:   Token.cpp
 * Author: mindfreakthemon
 * 
 * Created on September 27, 2012, 3:44 PM
 */

#include "Token.h"

Token::Token(int i) {
	type = i;
}

Token::~Token() {
}

int Token::getType() {
	return type;
}

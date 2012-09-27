/* 
 * File:   Token.cpp
 * Author: mindfreakthemon
 * 
 * Created on September 27, 2012, 3:44 PM
 */

#include "Token.h"

Token::Token(int type, int col, int row) {
	this->type = type;
	this->col = col;
	this->row = row;
}

Token::~Token() {
}

int Token::getType() {
	return type;
}

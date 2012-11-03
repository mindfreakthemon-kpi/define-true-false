/*
 * Token.h
 *
 *  Created on: 28.10.2012
 *      Author: CrashTUA
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>

namespace token {

enum TokenKind {
	LF_PARENTHESES = 1, //'('
	RT_PARENTHESES, //')'
	LF_CR_BRACKET, //'{'
	RT_CR_BRACKET, //'}'
	LF_SQ_BRACKET, //'['
	RT_SQ_BRACKET, //']'

	SEMICOLON, //';'
	COLON, //':'
	COMMA, //','

	LESS, //'<'
	MORE, //'>'

	LESS_EQUALS, //'<='
	MORE_EQUALS, //'>='

	PLUS, //'+'
	MINUS, //'-'
	MULT, //'*'
	DIVIDE, //'/'

	VAR,

	DOUBLE_QUOTE, //'"'
	QUOTE, //'\''

	CARET, //'^'
	EQUALS, //'='
	DEQUALS, //'=='
	NEQUALS, //'!='
	AND, //'&'
	OR, //'|'
	DAND, //'&&'
	DOR, //'||'
	NEGATION, //!
	TILDE, //~

	ID,
	FUNCTION,
	WHILE,
	RETURN,
	IF,
	ELSE,

	INT,
	DOUBLE,
	BOOL,
	STRING,

	OPERATOR,
	COMMENT,

	TYPE,

	LT //last token of file
};

enum DataType {
	NONE = 0, TYPE_INT, TYPE_DOUBLE, TYPE_STRING, TYPE_BOOL, TYPE_VOID
};

inline std::string getKindString(TokenKind kind) {
	switch(kind) {
		case LF_PARENTHESES:
			return std::string("LF_PARENTHESES");
		case RT_PARENTHESES:
			return std::string("RT_PARENTHESES");
		case LF_CR_BRACKET:
			return std::string("LF_CR_BRACKET");
		case RT_CR_BRACKET:
			return std::string("RT_CR_BRACKET");
		case LF_SQ_BRACKET:
			return std::string("LF_SQ_BRACKET");
		case RT_SQ_BRACKET:
			return std::string("RT_SQ_BRACKET");
	
		case SEMICOLON:
			return std::string("SEMICOLON");
		case COLON:
			return std::string("COLON");
		case COMMA:
			return std::string("COMMA");
	
		case LESS:
			return std::string("LESS");
		case MORE:
			return std::string("MORE");
	
		case LESS_EQUALS:
			return std::string("LESS_EQUALS");
		case MORE_EQUALS:
			return std::string("MORE_EQUALS");
	
		case PLUS:
			return std::string("PLUS");
		case MINUS:
			return std::string("MINUS");
		case MULT:
			return std::string("MULT");
		case DIVIDE:
			return std::string("DIVIDE");
	
		case DOUBLE_QUOTE:
			return std::string("DOUBLE_QUOTE");
		case QUOTE:
			return std::string("QUOTE");
	
		case CARET:
			return std::string("CARET");
		case EQUALS:
			return std::string("EQUALS");
		case DEQUALS:
			return std::string("DEQUALS");
		case NEQUALS:
			return std::string("NEQUALS");
		case AND:
			return std::string("AND");
		case OR:
			return std::string("OR");
		case DAND:
			return std::string("DAND");
		case DOR:
			return std::string("DOR");
		case NEGATION:
			return std::string("NEGATION");
		case TILDE:
			return std::string("TILDE");
	
		case ID:
			return std::string("ID");
		case FUNCTION:
			return std::string("FUNCTION");
		case WHILE:
			return std::string("WHILE");
		case RETURN:
			return std::string("RETURN");
	
		case INT:
			return std::string("INT");
		case DOUBLE:
			return std::string("DOUBLE");
		case BOOL:
			return std::string("BOOL");
		case STRING:
			return std::string("STRING");
	
		case OPERATOR:
			return std::string("OPERATOR");
		case COMMENT:
			return std::string("COMMENT");
	
		case TYPE:
			return std::string("TYPE");
		case VAR:
			return std::string("VAR");
		case ELSE:
			return std::string("ELSE");
		case IF:
			return std::string("IF");
	
		case LT:
			return std::string("LT"); //last token of file
	}
	
	assert(false);
};

inline std::string getSourceString(TokenKind kind) {
	switch(kind) {	
		case LESS:
			return "<";
		case MORE:
			return ">";
	
		case LESS_EQUALS:
			return "<=";
		case MORE_EQUALS:
			return ">=";
	
		case PLUS:
			return "+";
		case MINUS:
			return "-";
		case MULT:
			return "*";
		case DIVIDE:
			return "/";
	
		case CARET:
			return "^";
		//case EQUALS:
		//	return "=";
		case DEQUALS:
			return "==";
		case NEQUALS:
			return "!=";
		case AND:
			return "&";
		case OR:
			return "|";
		case DAND:
			return "&&";
		case DOR:
			return "||";
		case NEGATION:
			return "!";
		case TILDE:
			return "~";
	}
	
	assert(false);
};

inline std::string dataTypeString(DataType dT) {
	switch(dT) {
		case token::TYPE_INT: return "int";
		case token::TYPE_DOUBLE: return "double";
		case token::TYPE_STRING: return "string";
		case token::TYPE_BOOL: return "bool";
		case token::TYPE_VOID: return "void";
	}
	
	return "unknown";
}

}

struct SourceLocation {
	int row;
	int col;
	SourceLocation(int row, int col) :
			row(row), col(col) {
	}
	SourceLocation() :
			row(0), col(0) {
	}
	unsigned getLine() const {
		return row;
	}
	unsigned getColumn() const {
		return col;
	}
	std::string toString() {
		std::stringstream ss;
		ss << "row:" << row << ";col:" << col;
		return ss.str();
	}
};

class Token {
public:
	/**
	 * 	Default constructor
	 **/
	Token(token::TokenKind kind, SourceLocation loc) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					0), data_type(token::NONE) {
	}
	/**
	 * For STRING and ID
	 **/
	Token(token::TokenKind kind, SourceLocation loc, std::string string_data) :
			kind(kind), loc(loc), id(string_data), string_data(string_data), int_data(
					0), double_data(0), data_type(token::NONE) {
	}
	/**
	 * For INT,BOOL(int_data == 0 == false)
	 **/
	Token(token::TokenKind kind, SourceLocation loc, int int_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(int_data), double_data(
					0), data_type(token::NONE) {
	}
	/**
	 * For DOUBLE
	 **/
	Token(token::TokenKind kind, SourceLocation loc, double double_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					double_data), data_type(token::NONE) {
	}
	/**
	 * For TYPE_XXX(SCALAR,ARRAY):TYPE_XXX
	 **/
	Token(token::TokenKind kind, SourceLocation loc, token::DataType data_type) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					0), data_type(data_type) {
	}
	token::TokenKind getKind() const {
		return kind;
	}
	std::string getStringData() {
		return string_data;
	}
	std::string getID() {
		return string_data;
	}
	int getIntData() {
		return int_data;
	}
	double getDoubleData() {
		return double_data;
	}
	token::DataType getDataType() const {
		return data_type;
	}
	SourceLocation getLocation() const {
		return loc;
	}
	~Token() {
	}
private:
	token::TokenKind kind;
	SourceLocation loc;
	//id for ID tokens
	std::string id;
	//data for tokens with some data
	std::string string_data;
	int int_data;
	double double_data;
	token::DataType data_type;
};

#endif /* TOKEN_H_ */

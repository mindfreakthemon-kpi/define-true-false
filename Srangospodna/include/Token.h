/*
 * Token.h
 *
 *  Created on: 28.10.2012
 *      Author: CrashTUA
 */

#ifndef TOKEN_H_
#define TOKEN_H_
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
	AND, //'&'
	OR, //'|'
	DAND, //'&&'
	DOR, //'||'

	ID,
	FUNCTION,
	WHILE,
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
	Token(int kind, SourceLocation loc) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					0), data_type(token::NONE) {
	}
	/**
	 * For STRING and ID
	 **/
	Token(int kind, SourceLocation loc, std::string string_data) :
			kind(kind), loc(loc), id(string_data), string_data(string_data), int_data(
					0), double_data(0), data_type(token::NONE) {
	}
	/**
	 * For INT,BOOL(int_data == 0 == false)
	 **/
	Token(int kind, SourceLocation loc, int int_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(int_data), double_data(
					0), data_type(token::NONE) {
	}
	/**
	 * For DOUBLE
	 **/
	Token(int kind, SourceLocation loc, double double_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					double_data), data_type(token::NONE) {
	}
	/**
	 * For TYPE_XXX(SCALAR,ARRAY):TYPE_XXX
	 **/
	Token(int kind, SourceLocation loc, token::DataType data_type) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					0), data_type(data_type) {
	}
	unsigned getKind() const {
		return kind;
	}
	std::string getKindString() const {
		switch (kind) {
		case token::LF_PARENTHESES:
			return std::string("LF_PARENTHESES");
		case token::RT_PARENTHESES:
			return std::string("RT_PARENTHESES");
		case token::LF_CR_BRACKET:
			return std::string("LF_CR_BRACKET");
		case token::RT_CR_BRACKET:
			return std::string("RT_CR_BRACKET");
		case token::LF_SQ_BRACKET:
			return std::string("LF_SQ_BRACKET");
		case token::RT_SQ_BRACKET:
			return std::string("RT_SQ_BRACKET");

		case token::SEMICOLON:
			return std::string("SEMICOLON");
		case token::COLON:
			return std::string("COLON");
		case token::COMMA:
			return std::string("COMMA");

		case token::LESS:
			return std::string("LESS");
		case token::MORE:
			return std::string("MORE");

		case token::LESS_EQUALS:
			return std::string("LESS_EQUALS");
		case token::MORE_EQUALS:
			return std::string("MORE_EQUALS");

		case token::PLUS:
			return std::string("PLUS");
		case token::MINUS:
			return std::string("MINUS");
		case token::MULT:
			return std::string("MULT");
		case token::DIVIDE:
			return std::string("DIVIDE");

		case token::DOUBLE_QUOTE:
			return std::string("DOUBLE_QUOTE");
		case token::QUOTE:
			return std::string("QUOTE");

		case token::CARET:
			return std::string("CARET");
		case token::EQUALS:
			return std::string("EQUALS");
		case token::DEQUALS:
			return std::string("DEQUALS");
		case token::AND:
			return std::string("AND");
		case token::OR:
			return std::string("OR");
		case token::DAND:
			return std::string("DAND");
		case token::DOR:
			return std::string("DOR");

		case token::ID:
			return std::string("ID");
		case token::FUNCTION:
			return std::string("FUNCTION");
		case token::WHILE:
			return std::string("WHILE");

		case token::INT:
			return std::string("INT");
		case token::DOUBLE:
			return std::string("DOUBLE");
		case token::BOOL:
			return std::string("BOOL");
		case token::STRING:
			return std::string("STRING");

		case token::OPERATOR:
			return std::string("OPERATOR");
		case token::COMMENT:
			return std::string("COMMENT");

		case token::TYPE:
			return std::string("TYPE");
		case token::VAR:
			return std::string("VAR");
		case token::ELSE:
			return std::string("ELSE");
		case token::IF:
			return std::string("IF");

		case token::LT:
			return std::string("LT"); //last token of file
		}

		assert(false && "unreachable");
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
	SourceLocation getLocation() const {
		return loc;
	}
	~Token() {
	}
private:
	int kind;
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

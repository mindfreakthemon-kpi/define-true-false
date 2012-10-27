/* 
 * File:   Lexer.h
 * Author: CrashTUA
 *
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#ifndef LEXER_H
#define	LEXER_H
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
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
					0), data_type(NONE) {
	}
	/**
	 * For STRING and ID
	 **/
	Token(int kind, SourceLocation loc, std::string string_data) :
			kind(kind), loc(loc), id(string_data), string_data(string_data), int_data(
					0), double_data(0), data_type(NONE) {
	}
	/**
	 * For INT,BOOL(int_data == 0 == false)
	 **/
	Token(int kind, SourceLocation loc, int int_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(int_data), double_data(
					0), data_type(NONE) {
	}
	/**
	 * For DOUBLE
	 **/
	Token(int kind, SourceLocation loc, double double_data) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					double_data), data_type(NONE) {
	}
	/**
	 * For TYPE_XXX(SCALAR,ARRAY):TYPE_XXX
	 **/
	Token(int kind, SourceLocation loc, DataType data_type) :
			kind(kind), loc(loc), id(""), string_data(""), int_data(0), double_data(
					0), data_type(data_type) {
	}
	unsigned getKind() const {
		return kind;
	}
	string getKindString() const {
		switch (kind) {
		case LF_PARENTHESES:
			return string("LF_PARENTHESES");
		case RT_PARENTHESES:
			return string("RT_PARENTHESES");
		case LF_CR_BRACKET:
			return string("LF_CR_BRACKET");
		case RT_CR_BRACKET:
			return string("RT_CR_BRACKET");
		case LF_SQ_BRACKET:
			return string("LF_SQ_BRACKET");
		case RT_SQ_BRACKET:
			return string("RT_SQ_BRACKET");

		case SEMICOLON:
			return string("SEMICOLON");
		case COLON:
			return string("COLON");
		case COMMA:
			return string("COMMA");

		case LESS:
			return string("LESS");
		case MORE:
			return string("MORE");

		case LESS_EQUALS:
			return string("LESS_EQUALS");
		case MORE_EQUALS:
			return string("MORE_EQUALS");

		case PLUS:
			return string("PLUS");
		case MINUS:
			return string("MINUS");
		case MULT:
			return string("MULT");
		case DIVIDE:
			return string("DIVIDE");

		case DOUBLE_QUOTE:
			return string("DOUBLE_QUOTE");
		case QUOTE:
			return string("QUOTE");

		case CARET:
			return string("CARET");
		case EQUALS:
			return string("EQUALS");
		case DEQUALS:
			return string("DEQUALS");
		case AND:
			return string("AND");
		case OR:
			return string("OR");
		case DAND:
			return string("DAND");
		case DOR:
			return string("DOR");

		case ID:
			return string("ID");
		case FUNCTION:
			return string("FUNCTION");
		case WHILE:
			return string("WHILE");

		case INT:
			return string("INT");
		case DOUBLE:
			return string("DOUBLE");
		case BOOL:
			return string("BOOL");
		case STRING:
			return string("STRING");

		case OPERATOR:
			return string("OPERATOR");
		case COMMENT:
			return string("COMMENT");

		case TYPE:
			return string("TYPE");
		case VAR:
			return string("VAR");
		case ELSE:
			return string("ELSE");
		case IF:
			return string("IF");

		case LT:
			return string("LT"); //last token of file
		}

		assert(false && "unreachable");
	}
	std::string getStringData() {
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
	DataType data_type;
};
}

namespace lexer {
using namespace token;
class Lexer {
public:
	Lexer(string source);
	token::Token* scan();
	~Lexer() {
	}
private:
	bool isValidIdChar(int c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
	}
	int file_length;
	string file_buffer;
	ifstream file_is;
	int col;
	int row;
	int charsDone;
};
}
#endif	/* LEXER_H */


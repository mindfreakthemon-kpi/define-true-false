/* 
 * File:   Token.h
 * Author: mindfreakthemon
 *
 * Created on September 27, 2012, 3:44 PM
 */

#ifndef TOKEN_H
#define	TOKEN_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
namespace token {
	enum TokenKind {
		LF_PARENTHESES = 1,//'('
		RT_PARENTHESES, //')'
		LF_CR_BRACKET,//'{'
		RT_CR_BRACKET,//'}'
		LF_SQ_BRACKET,//'['
		RT_SQ_BRACKET,//']'

		SEMICOLON,//';'
		COLON,//':'
		COMMA,//','

		LESS,//'<'
		MORE,//'>'

		LESS_EQUALS,//'<='
		MORE_EQUALS,//'>='

		PLUS,//'+'
		MINUS,//'-'
		MULT,//'*'
		DIVIDE,//'/'
		CARET,//'^'
		EQUALS,//'='

		DEQUALS,//'=='

		DOUBLE_QUOTE,//'"'
		QUOTE,//'\''

		AND,//'&'
		OR,//'|'

		DAND,//'&&'
		DOR,//'||'

		ID,

		INT,
		DOUBLE,
		STRING,
		BOOL,
		VOID,

		FUNCTION,
		WHILE,

		OPERATOR,
		COMMENT,
	
		TYPE_SCALAR,
		TYPE_ARRAY,

		FT,//first token of file, root token
		LT //last token of file
	};
	enum DataType {
		NONE = 0,
		DINT,
		DDOUBLE,
		DSTRING,
		DBOOL,
		DVOID
	};
	struct SourceLocation {
		int row;
		int col;
		SourceLocation(int row,int col):row(row),col(col){}
		SourceLocation(){row = 0; col = 0;}
		std::string toString(){
			std::stringstream ss;
			ss << "row:" << row << ";col:" << col;
			return ss.str();
		}
	};
	class Token {
	public:
		Token(int kind, SourceLocation loc):
			_kind(kind),_loc(loc),id(""),_string_data(""),
			_int_data(0),_double_data(0),parent(NULL),_data_type(NONE){}
		//constructor for STRING and ID tokens
		Token(int kind, SourceLocation loc,std::string string_data):
			_kind(kind),_loc(loc),id(string_data),_string_data(string_data),
			_int_data(0),_double_data(0),parent(NULL),_data_type(NONE){}
		Token(int kind, SourceLocation loc,int int_data):
			_kind(kind),_loc(loc),id(""),_string_data(""),
			_int_data(int_data),_double_data(0),parent(NULL),_data_type(NONE){}
		Token(int kind, SourceLocation loc,double double_data):
			_kind(kind),_loc(loc),id(""),_string_data(""),
			_int_data(0),_double_data(double_data),parent(NULL),_data_type(NONE){}
		Token(int kind, SourceLocation loc,DataType data_type):
			_kind(kind),_loc(loc),id(""),_string_data(""),
			_int_data(0),_double_data(0),parent(NULL),_data_type(data_type){}
		int getKind() {
			return _kind;
		}
		std::string getStringData(){
			return _string_data;
		}
		int getIntData(){
			return _int_data;
		}
		double getDoubleData(){
			return _double_data;
		}
		std::vector<Token*> & getLeafs(){
			return leafs;
		}
		Token* getParent(){
			return parent;
		}
		void setParent(Token* parent){
			this->parent = parent;
		}
		SourceLocation & getLocation(){
			return _loc;
		}
		~Token() {
		}
	private:
		int _kind;
		SourceLocation _loc;
		//id for ID tokens
		std::string id;
		//data for tokens with some data
		std::string _string_data;
		int _int_data;
		double _double_data;
		std::vector<Token*> leafs;
		Token* parent;
		DataType _data_type;
	};
}

#endif	/* TOKEN_H */


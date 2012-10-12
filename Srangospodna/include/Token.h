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
		
		PLUS,//'+'
		MINUS,//'-'
		MULT,//'*'
		DIVIDE,//'/'
		CARET,//'^'
		
		DOUBLE_QUOTE,//'"'
		QUOTE,//'\''

		ID,
		INT,
		DOUBLE,
		STRING,
		BOOL,
	
		OPERATOR,
		COMMENT,
	
		TYPE_SCALAR,
		TYPE_ARRAY,

		LT //last token of file
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
		//private section first because of MINGW warning "when initialized here" o_O
		//this warning related with a list of initialization and field order
		//the other solution - change list of initialization:)
	private:
		int _kind;

	public:
		Token(int kind, SourceLocation loc):_kind(kind),_loc(loc),_string_data(""){}
		Token(int kind, SourceLocation loc,std::string string_data):_kind(kind),_loc(loc),_string_data(string_data){}
		int getKind() {
			return _kind;
		}
		~Token() {
		}
		SourceLocation _loc;
		std::string _string_data;
		//TODO add pointers to other data, like int, double...

	};
}

#endif	/* TOKEN_H */


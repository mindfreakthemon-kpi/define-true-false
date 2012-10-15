/* 
 * File:   Lexer.cpp
 * Author: CrashTUA
 * 
 * Created on 27 Сентябрь 2012 г., 18:54
 */

#include <stddef.h>

#include "Lexer.h"
using namespace token;
using namespace lexer;
const int Lexer::_EOF = -1050;
const int Lexer::_ENDL = -5010;
Lexer::Lexer(std::string file) {
	file_is.open(file.c_str(), ios::binary);
	file_is.seekg(0, ios::end);
	file_length = file_is.tellg();
	file_is.seekg(0, ios::beg);
	file_buffer = new char[file_length];
	file_is.read(file_buffer, file_length);
	file_is.close();
	col = 1;
	row = 1;
	charsDone = -1;
}

Token* Lexer::scan() {
	//for multi symbol tokens
	std::string nts;

	//read character
	int c = getNextChar();

	//some serious stuff
	switch (c) {
	case '{':
		return new Token(LF_CR_BRACKET, SourceLocation(row, col));
	case '}':
		return new Token(RT_CR_BRACKET, SourceLocation(row, col));
	case '(':
		return new Token(LF_PARENTHESES, SourceLocation(row, col));
	case ')':
		return new Token(RT_PARENTHESES, SourceLocation(row, col));
	case '[':
		return new Token(LF_SQ_BRACKET, SourceLocation(row, col));
	case ']':
		return new Token(RT_SQ_BRACKET, SourceLocation(row, col));
	case ';':
		return new Token(SEMICOLON, SourceLocation(row, col));
	case ':':
		return new Token(COLON, SourceLocation(row, col));
	case ',':
		return new Token(COMMA, SourceLocation(row, col));
	//here goes operators
	case '<':
		return new Token(LESS, SourceLocation(row, col));
	case '>':
		return new Token(MORE, SourceLocation(row, col));
	case '+':
		return new Token(PLUS, SourceLocation(row, col));
	case '-':
		return new Token(MINUS, SourceLocation(row, col));
	case '*':
		return new Token(MULT, SourceLocation(row, col));
	case '/':
		return new Token(DIVIDE, SourceLocation(row, col));
	case '^':
		return new Token(CARET, SourceLocation(row, col));
	case '=':
		if(file_buffer[charsDone+1] == '='){
			getNextChar();
			return new Token(DEQUALS, SourceLocation(row, col));
		}
		else
			return new Token(EQUALS, SourceLocation(row, col));
	case '&':
		return new Token(AND, SourceLocation(row, col));
	case '|':
		return new Token(OR, SourceLocation(row, col));
		//STRING
	case '"': {
		SourceLocation pos(row, col);
		do {
			c = getNextChar();
			if (c == '\\') {
				c = getNextChar();
				nts.push_back(c);
				//TODO process escaped symbols
			}
			if (c != '"')
				nts.push_back(c);
		} while (c != '"');
		return new Token(STRING, pos, nts);
	}
	case Lexer::_EOF:
		return new Token(LT, SourceLocation(row, col));
	default: {
		//here we will try to get keyword or id; tree must be created on higher level
		LexerDictionary & dic = LexerDictionary::getInstance();
		//return new Token(FUNCTION,SourceLocation(row, col));
		//return new Token(ID,SourceLocation(row, col),nts);
		bool done = false;
		while (!done) {
			nts.push_back(c);
			c = getNextChar(false);
			done = !dic.isValidIdChar(c);
			if (!done)
				c = getNextChar();
		}
		//here we have ID in nts, BUT it can be also keyword or type
		//keywords.push_back("function");
		//keywords.push_back("while");
		if(dic.findType(nts)){
//			DINT,
//			DDOUBLE,
//			DSTRING,
//			DVOID
			if(file_buffer[charsDone+1] == '[' && file_buffer[charsDone+2] == ']')
			{
				if(nts.compare("int"))
					return new Token(TYPE_SCALAR,SourceLocation(row, col),DINT);
				if(nts.compare("double"))
					return new Token(TYPE_SCALAR,SourceLocation(row, col),DDOUBLE);
				if(nts.compare("string"))
					return new Token(TYPE_SCALAR,SourceLocation(row, col),DSTRING);
				if(nts.compare("bool"))
					return new Token(TYPE_SCALAR,SourceLocation(row, col),DBOOL);
				if(nts.compare("void"))
					return new Token(TYPE_SCALAR,SourceLocation(row, col),DVOID);
			}
			else
			{
				if(nts.compare("int"))
					return new Token(TYPE_ARRAY,SourceLocation(row, col),DINT);
				if(nts.compare("double"))
					return new Token(TYPE_ARRAY,SourceLocation(row, col),DDOUBLE);
				if(nts.compare("string"))
					return new Token(TYPE_ARRAY,SourceLocation(row, col),DSTRING);
				if(nts.compare("bool"))
					return new Token(TYPE_ARRAY,SourceLocation(row, col),DBOOL);
				if(nts.compare("void"))
					return new Token(TYPE_ARRAY,SourceLocation(row, col),DVOID);
			}

		}
		if (dic.findKeyword(nts)){
			if(nts.compare("function"))
				return new Token(FUNCTION,SourceLocation(row, col));
			if(nts.compare("while"))
				return new Token(WHILE,SourceLocation(row, col));
		}
		return new Token(ID,SourceLocation(row, col),nts);
		break;
	}
	}
	return NULL;
}

int Lexer::getNextChar(bool doIncrement) {
	if (charsDone >= file_length)
		return Lexer::_EOF;
	if (doIncrement) {
		charsDone++;
		col++;
		//for DOS line endings
		if (file_buffer[charsDone] == '\r'
				&& file_buffer[charsDone + 1] == '\n') {
			row++;
			col = 0;
			charsDone++;
			return Lexer::_ENDL;
		}
		//for *nix line endings
		if (file_buffer[charsDone] == '\n') {
			row++;
			col = 0;
			return Lexer::_ENDL;
		}
	} else {
		return file_buffer[charsDone + 1];//return next char, but not increment charsDone & col & row
	}
	return file_buffer[charsDone];
}
bool Lexer::foundChar(char c, bool allowWhitespace) {
	bool done = false;
	int i = 1;
	while(!done){
		if(allowWhitespace == true){
			if(file_buffer[charsDone+i] == c)
				return true;
			else if(file_buffer[charsDone+i] != ' ')
				return false;//char not found
		}
		else
			if(file_buffer[charsDone+i] != c)
				return false;
		i++;
	}
	return false;
}

LexerDictionary * lexer::LexerDictionary::instance = NULL;
LexerDictionary& LexerDictionary::getInstance() {
	if(lexer::LexerDictionary::instance == NULL){
		lexer::LexerDictionary::instance = new LexerDictionary();
	}
	return *LexerDictionary::instance;
}

bool LexerDictionary::findKeyword(string word) {
	std::vector<string>::iterator itr;
	for ( itr = keywords.begin(); itr < keywords.end(); ++itr )
	{
		if((*itr).compare(word) == 0)
			return true;
	}
	return false;
}

bool LexerDictionary::findOperator(string word) {
	std::vector<string>::iterator itr;
	for ( itr = operators.begin(); itr < operators.end(); ++itr )
	{
		if((*itr).compare(word) == 0)
			return true;
	}
	return false;
}

bool LexerDictionary::findType(string word) {
	std::vector<string>::iterator itr;
	for ( itr = types.begin(); itr < types.end(); ++itr )
	{
		if((*itr).compare(word) == 0)
			return true;
	}
	return false;
}

LexerDictionary::LexerDictionary() {
	keywords.push_back("function");
	keywords.push_back("while");

	types.push_back("int");
	types.push_back("double");
	types.push_back("string");
	types.push_back("bool");
	types.push_back("void");

	operators.push_back("=");
	operators.push_back("==");
	operators.push_back("-");
	operators.push_back("+");
	operators.push_back("-");
	operators.push_back("/");
	operators.push_back(">");
	operators.push_back("<");
	operators.push_back("!=");
	operators.push_back(">=");
	operators.push_back("<=");
	operators.push_back("&");
	operators.push_back("&&");
	operators.push_back("|");
	operators.push_back("||");
}

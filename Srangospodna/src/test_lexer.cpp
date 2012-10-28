#include <cstddef>
#include <vector>
#include "gtest/gtest.h"
#include <Lexer.h>
#include <iostream>
using namespace std;
//using namespace token;
using namespace lexer;
using token::Token;
using token::SourceLocation;
using token::TokenKind;
namespace {

vector<Token> tokenize(string source) {
	Lexer l(source);
	Token* t = NULL;
	vector<Token> result;

	while ((t = l.scan())->getKind() != token::LT) {
		result.push_back(*t);
	}

	return result;
}

::testing::AssertionResult tok_is(const Token &tok, TokenKind kind) {
	if (tok.getKind() != kind)
		return testing::AssertionFailure() << "Expected token kind " << kind
				<< ", got " << tok.getKind();

	return testing::AssertionSuccess();
}

::testing::AssertionResult tok_is(const Token &tok, TokenKind kind,
		unsigned line, unsigned column) {
	if (tok.getKind() != kind)
		return testing::AssertionFailure() << "Expected token kind " << kind
				<< ", got " << tok.getKind();

	if (tok.getLocation().getLine() != line)
		return testing::AssertionFailure() << "Expected line " << line
				<< ", got " << tok.getLocation().getLine();

	if (tok.getLocation().getColumn() != column)
		return testing::AssertionFailure() << "Expected column " << column
				<< ", got " << tok.getLocation().getColumn();

	return testing::AssertionSuccess();
}

} // unnamed namespace

TEST(Lexer, TokAllParens) {
	string source = "()[]{}";
	vector<Token> toks = tokenize(source);
	ASSERT_EQ(6U, toks.size());
	ASSERT_TRUE(tok_is(toks[0], token::LF_PARENTHESES, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], token::RT_PARENTHESES, 1, 2));
	ASSERT_TRUE(tok_is(toks[2], token::LF_SQ_BRACKET, 1, 3));
	ASSERT_TRUE(tok_is(toks[3], token::RT_SQ_BRACKET, 1, 4));
	ASSERT_TRUE(tok_is(toks[4], token::LF_CR_BRACKET, 1, 5));
	ASSERT_TRUE(tok_is(toks[5], token::RT_CR_BRACKET, 1, 6));
}

TEST(Lexer, Ololo) {
	string source =
			"function name(var1:int[],var2:double,var3:string):void{ var i: int, v: double; i = 2; while(0){ i = 3;}}";
	vector<Token> toks = tokenize(source);
	ASSERT_EQ(44U, toks.size());
	ASSERT_TRUE(tok_is(toks[0], token::FUNCTION, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], token::ID, 1, 10));
	ASSERT_TRUE(tok_is(toks[2], token::LF_PARENTHESES, 1, 14));
	ASSERT_TRUE(tok_is(toks[3], token::ID, 1, 15));
	ASSERT_TRUE(tok_is(toks[4], token::COLON, 1, 19));
	ASSERT_TRUE(tok_is(toks[5], token::TYPE, 1, 20));
	ASSERT_TRUE(tok_is(toks[6], token::COMMA, 1, 25));
	ASSERT_TRUE(tok_is(toks[7], token::ID, 1, 26));
	ASSERT_TRUE(tok_is(toks[8], token::COLON, 1, 30));
	ASSERT_TRUE(tok_is(toks[9], token::TYPE, 1, 31));
	ASSERT_TRUE(tok_is(toks[10], token::COMMA, 1, 37));
	ASSERT_TRUE(tok_is(toks[11], token::ID, 1, 38));
	ASSERT_TRUE(tok_is(toks[12], token::COLON, 1, 42));
	ASSERT_TRUE(tok_is(toks[13], token::TYPE, 1, 43));
	ASSERT_TRUE(tok_is(toks[14], token::RT_PARENTHESES, 1, 49));
	ASSERT_TRUE(tok_is(toks[15], token::COLON, 1, 50));
	ASSERT_TRUE(tok_is(toks[16], token::TYPE, 1, 51));
	ASSERT_TRUE(tok_is(toks[17], token::LF_CR_BRACKET, 1, 55));
	ASSERT_TRUE(tok_is(toks[18], token::VAR, 1, 57));
	ASSERT_TRUE(tok_is(toks[19], token::ID, 1, 61));
	ASSERT_TRUE(tok_is(toks[20], token::COLON, 1, 62));
	ASSERT_TRUE(tok_is(toks[21], token::TYPE, 1, 64));
	ASSERT_TRUE(tok_is(toks[22], token::COMMA, 1, 67));
	ASSERT_TRUE(tok_is(toks[23], token::ID, 1, 69));
	ASSERT_TRUE(tok_is(toks[24], token::COLON, 1, 70));
	ASSERT_TRUE(tok_is(toks[25], token::TYPE, 1, 72));
	ASSERT_TRUE(tok_is(toks[26], token::SEMICOLON, 1, 78));
	ASSERT_TRUE(tok_is(toks[27], token::ID, 1, 80));
	ASSERT_TRUE(tok_is(toks[28], token::EQUALS, 1, 82));
	ASSERT_TRUE(tok_is(toks[29], token::INT, 1, 84));
	ASSERT_TRUE(tok_is(toks[30], token::SEMICOLON, 1, 85));
	ASSERT_TRUE(tok_is(toks[31], token::WHILE, 1, 87));
	ASSERT_TRUE(tok_is(toks[32], token::LF_PARENTHESES, 1, 92));
	ASSERT_TRUE(tok_is(toks[33], token::INT, 1, 93));
	ASSERT_TRUE(tok_is(toks[34], token::RT_PARENTHESES, 1, 94));
	ASSERT_TRUE(tok_is(toks[35], token::LF_CR_BRACKET, 1, 95));
	ASSERT_TRUE(tok_is(toks[36], token::ID, 1, 97));
	ASSERT_TRUE(tok_is(toks[37], token::EQUALS, 1, 99));
	ASSERT_TRUE(tok_is(toks[38], token::INT, 1, 101));
	ASSERT_TRUE(tok_is(toks[39], token::SEMICOLON, 1, 102));
	ASSERT_TRUE(tok_is(toks[40], token::RT_CR_BRACKET, 1, 103));
	ASSERT_TRUE(tok_is(toks[41], token::RT_CR_BRACKET, 1, 104));
}

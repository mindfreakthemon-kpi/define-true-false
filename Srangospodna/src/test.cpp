#include <cstddef>
#include <vector>
#include "gtest/gtest.h"
#include <Lexer.h>
#include <iostream>
using namespace std;
//using namespace token;
using namespace lexer;

namespace {

template<typename T, size_t N>
size_t array_lengthof(T (&array)[N]) {
	return N;
}

vector<Token> tokenize(string source) {
	Lexer l(source);
	Token* t = NULL;
	vector<Token> result;

	while ((t = l.scan())->getKind() != LT) {
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
	ASSERT_TRUE(tok_is(toks[0], LF_PARENTHESES, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], RT_PARENTHESES, 1, 2));
	ASSERT_TRUE(tok_is(toks[2], LF_SQ_BRACKET, 1, 3));
	ASSERT_TRUE(tok_is(toks[3], RT_SQ_BRACKET, 1, 4));
	ASSERT_TRUE(tok_is(toks[4], LF_CR_BRACKET, 1, 5));
	ASSERT_TRUE(tok_is(toks[5], RT_CR_BRACKET, 1, 6));
}

TEST(Lexer, Ololo) {
	string source =
			"function name(var1:int[],var2:double,var3:string):void{ var i: int, v: double; i = 2; while(0){ i = 3;}}";
	vector<Token> toks = tokenize(source);
	ASSERT_EQ(44U, toks.size());
	ASSERT_TRUE(tok_is(toks[0], FUNCTION, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], ID, 1, 10));
	ASSERT_TRUE(tok_is(toks[2], LF_PARENTHESES, 1, 14));
	ASSERT_TRUE(tok_is(toks[3], ID, 1, 15));
	ASSERT_TRUE(tok_is(toks[4], COLON, 1, 19));
	ASSERT_TRUE(tok_is(toks[5], TYPE, 1, 20));
	ASSERT_TRUE(tok_is(toks[6], COMMA, 1, 25));
	ASSERT_TRUE(tok_is(toks[7], ID, 1, 26));
	ASSERT_TRUE(tok_is(toks[8], COLON, 1, 30));
	ASSERT_TRUE(tok_is(toks[9], TYPE, 1, 31));
	ASSERT_TRUE(tok_is(toks[10], COMMA, 1, 37));
	ASSERT_TRUE(tok_is(toks[11], ID, 1, 38));
	ASSERT_TRUE(tok_is(toks[12], COLON, 1, 42));
	ASSERT_TRUE(tok_is(toks[13], TYPE, 1, 43));
	ASSERT_TRUE(tok_is(toks[14], RT_PARENTHESES, 1, 49));
	ASSERT_TRUE(tok_is(toks[15], COLON, 1, 50));
	ASSERT_TRUE(tok_is(toks[16], TYPE, 1, 51));
	ASSERT_TRUE(tok_is(toks[17], LF_CR_BRACKET, 1, 55));
	ASSERT_TRUE(tok_is(toks[18], VAR, 1, 57));
	ASSERT_TRUE(tok_is(toks[19], ID, 1, 61));
	ASSERT_TRUE(tok_is(toks[20], COLON, 1, 62));
	ASSERT_TRUE(tok_is(toks[21], TYPE, 1, 64));
	ASSERT_TRUE(tok_is(toks[22], COMMA, 1, 67));
	ASSERT_TRUE(tok_is(toks[23], ID, 1, 69));
	ASSERT_TRUE(tok_is(toks[24], COLON, 1, 70));
	ASSERT_TRUE(tok_is(toks[25], TYPE, 1, 72));
	ASSERT_TRUE(tok_is(toks[26], SEMICOLON, 1, 78));
	ASSERT_TRUE(tok_is(toks[27], ID, 1, 80));
	ASSERT_TRUE(tok_is(toks[28], EQUALS, 1, 82));
	ASSERT_TRUE(tok_is(toks[29], INT, 1, 84));
	ASSERT_TRUE(tok_is(toks[30], SEMICOLON, 1, 85));
	ASSERT_TRUE(tok_is(toks[31], WHILE, 1, 87));
	ASSERT_TRUE(tok_is(toks[32], LF_PARENTHESES, 1, 92));
	ASSERT_TRUE(tok_is(toks[33], INT, 1, 93));
	ASSERT_TRUE(tok_is(toks[34], RT_PARENTHESES, 1, 94));
	ASSERT_TRUE(tok_is(toks[35], LF_CR_BRACKET, 1, 95));
	ASSERT_TRUE(tok_is(toks[36], ID, 1, 97));
	ASSERT_TRUE(tok_is(toks[37], EQUALS, 1, 99));
	ASSERT_TRUE(tok_is(toks[38], INT, 1, 101));
	ASSERT_TRUE(tok_is(toks[39], SEMICOLON, 1, 102));
	ASSERT_TRUE(tok_is(toks[40], RT_CR_BRACKET, 1, 103));
	ASSERT_TRUE(tok_is(toks[41], RT_CR_BRACKET, 1, 104));
}

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
	size_t array_lengthof(T (&array)[N])
	{
		return N;
	}

	vector<Token> tokenize(string source)
	{
		Lexer l(source);
		Token* t = NULL;
		vector<Token> result;

		while((t = l.scan())->getKind() != TokenKind::LT)
		{
			result.push_back(*t);
		}

		return result;
	}

	::testing::AssertionResult tok_is(const Token &tok,
		TokenKind kind)
	{
		if(tok.getKind() != kind)
			return testing::AssertionFailure() << "Expected token kind " << kind
			<< ", got " << tok.getKind();

		return testing::AssertionSuccess();
	}

	::testing::AssertionResult tok_is(const Token &tok,
		TokenKind kind,
		unsigned line,
		unsigned column)
	{
		if(tok.getKind() != kind)
			return testing::AssertionFailure() << "Expected token kind " << kind
			<< ", got " << tok.getKind();

		if(tok.getLocation().getLine() != line)
			return testing::AssertionFailure() << "Expected line " << line
			<< ", got " << tok.getLocation().getLine();

		if(tok.getLocation().getColumn() != column)
			return testing::AssertionFailure() << "Expected column " << column
			<< ", got " << tok.getLocation().getColumn();

		return testing::AssertionSuccess();
	}

} // unnamed namespace


TEST(Lexer, TokAllParens)
{
	string source = "()[]{}";
	vector<Token> toks = tokenize(source);
	ASSERT_EQ(6U, toks.size());
	ASSERT_TRUE(tok_is(toks[0], TokenKind::LF_PARENTHESES,   1, 1));
	ASSERT_TRUE(tok_is(toks[1], TokenKind::RT_PARENTHESES,   1, 2));
	ASSERT_TRUE(tok_is(toks[2], TokenKind::LF_SQ_BRACKET, 1, 3));
	ASSERT_TRUE(tok_is(toks[3], TokenKind::RT_SQ_BRACKET, 1, 4));
	ASSERT_TRUE(tok_is(toks[4], TokenKind::LF_CR_BRACKET,   1, 5));
	ASSERT_TRUE(tok_is(toks[5], TokenKind::RT_CR_BRACKET,   1, 6));
}

TEST(Lexer, Ololo)
{
	string source = "function name(var1:int[],var2:double,var3:string):void{ var i: int, v: double; i = 2; while(0){ i = 3;}}";
	vector<Token> toks = tokenize(source);
	ASSERT_EQ(42U, toks.size());
	ASSERT_TRUE(tok_is(toks[0], TokenKind::FUNCTION, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], TokenKind::ID, 1, 10));
	ASSERT_TRUE(tok_is(toks[2], TokenKind::LF_PARENTHESES, 1, 14));
	ASSERT_TRUE(tok_is(toks[3], TokenKind::ID, 1, 15));
	ASSERT_TRUE(tok_is(toks[4], TokenKind::COLON, 1, 19));
	ASSERT_TRUE(tok_is(toks[5], TokenKind::TYPE_ARRAY, 1, 20));
	ASSERT_TRUE(tok_is(toks[6], TokenKind::COMMA, 1, 25));
	ASSERT_TRUE(tok_is(toks[7], TokenKind::ID, 1, 26));
	ASSERT_TRUE(tok_is(toks[8], TokenKind::COLON, 1, 30));
	ASSERT_TRUE(tok_is(toks[9], TokenKind::TYPE_SCALAR, 1, 31));
	ASSERT_TRUE(tok_is(toks[10], TokenKind::COMMA, 1, 37));
	ASSERT_TRUE(tok_is(toks[11], TokenKind::ID, 1, 38));
	ASSERT_TRUE(tok_is(toks[12], TokenKind::COLON, 1, 42));
	ASSERT_TRUE(tok_is(toks[13], TokenKind::TYPE_SCALAR, 1, 43));
	ASSERT_TRUE(tok_is(toks[14], TokenKind::RT_PARENTHESES, 1, 49));
	ASSERT_TRUE(tok_is(toks[15], TokenKind::COLON, 1, 50));
	ASSERT_TRUE(tok_is(toks[16], TokenKind::TYPE_SCALAR, 1, 51));
	ASSERT_TRUE(tok_is(toks[17], TokenKind::LF_CR_BRACKET, 1, 55));
	ASSERT_TRUE(tok_is(toks[18], TokenKind::VAR, 1, 57));
	ASSERT_TRUE(tok_is(toks[19], TokenKind::ID, 1, 61));
	ASSERT_TRUE(tok_is(toks[20], TokenKind::COLON, 1, 62));
	ASSERT_TRUE(tok_is(toks[21], TokenKind::TYPE_SCALAR, 1, 64));
	ASSERT_TRUE(tok_is(toks[22], TokenKind::COMMA, 1, 67));
	ASSERT_TRUE(tok_is(toks[23], TokenKind::ID, 1, 69));
	ASSERT_TRUE(tok_is(toks[24], TokenKind::COLON, 1, 70));
	ASSERT_TRUE(tok_is(toks[25], TokenKind::TYPE_SCALAR, 1, 72));
	ASSERT_TRUE(tok_is(toks[26], TokenKind::SEMICOLON, 1, 78));
	ASSERT_TRUE(tok_is(toks[27], TokenKind::ID, 1, 80));
	ASSERT_TRUE(tok_is(toks[28], TokenKind::EQUALS, 1, 82));
	ASSERT_TRUE(tok_is(toks[29], TokenKind::INT, 1, 84));
	ASSERT_TRUE(tok_is(toks[30], TokenKind::SEMICOLON, 1, 85));
	ASSERT_TRUE(tok_is(toks[31], TokenKind::WHILE, 1, 87));
	ASSERT_TRUE(tok_is(toks[32], TokenKind::LF_PARENTHESES, 1, 92));
	ASSERT_TRUE(tok_is(toks[33], TokenKind::INT, 1, 93));
	ASSERT_TRUE(tok_is(toks[34], TokenKind::RT_PARENTHESES, 1, 94));
	ASSERT_TRUE(tok_is(toks[35], TokenKind::LF_CR_BRACKET, 1, 95));
	ASSERT_TRUE(tok_is(toks[36], TokenKind::ID, 1, 97));
	ASSERT_TRUE(tok_is(toks[37], TokenKind::EQUALS, 1, 99));
	ASSERT_TRUE(tok_is(toks[38], TokenKind::INT, 1, 101));
	ASSERT_TRUE(tok_is(toks[39], TokenKind::SEMICOLON, 1, 102));
	ASSERT_TRUE(tok_is(toks[40], TokenKind::RT_CR_BRACKET, 1, 103));
	ASSERT_TRUE(tok_is(toks[41], TokenKind::RT_CR_BRACKET, 1, 104));
}

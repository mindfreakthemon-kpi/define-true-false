#include <cstddef>
#include <vector>
#include "gtest/gtest.h"
#include <Lexer.h>
using namespace std;
using namespace token;
using namespace lexer;

namespace {

	template<typename T, size_t N>
	size_t array_lengthof(T (&array)[N])
	{
	  return N;
	}

	vector<Token> tokenize(string source)
	{
	  Lexer l(source, INLINE_STRING);
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


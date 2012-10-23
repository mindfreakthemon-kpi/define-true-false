#include <cstddef>
#include <vector>
#include "gtest/gtest.h"

#include <Token.h>
#include <Lexer.h>
using namespace std;
using namespace token;
using namespace lexer;

using std::vector;

namespace {

	template<typename T, size_t N>
	size_t array_lengthof(T (&array)[N])
	{
	  return N;
	}

	vector<Token> tokenize(const char *source)
	{
	  Lexer l(source);
	  vector<Token> result;

	  //while(l.next_is_not(TokenKind::LT))
	  //{
	  //  result.push_back(l.look());
	  //  l.consume_token();
	  //}

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



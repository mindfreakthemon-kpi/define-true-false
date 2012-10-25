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

	vector<Token> tokenize(const char *str)
	{
		string source(str);
	  Lexer l(source, INLINE_STRING);
    Token* t = NULL;
	  vector<Token> result;

	  while((t = l.scan())->getKind() != TokenKind::LT)
	  {
	    result.push_back(*t);
	  }

	  return result;
	}

	::testing::AssertionResult tok_is(Token &tok,
		                          TokenKind kind)
	{
	  if(tok.getKind() != kind)
	    return testing::AssertionFailure() << "Expected token kind " << kind
	      << ", got " << tok.getKind();

	  return testing::AssertionSuccess();
	}

	::testing::AssertionResult tok_is(Token &tok,
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



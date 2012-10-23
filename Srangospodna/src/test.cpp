#include <cstddef>
#include <vector>
#include "gtest/gtest.h"
#include "lexer.h"

using std::vector;

namespace {

	template<typename T, size_t N>
	size_t array_lengthof(T (&array)[N])
	{
	  return N;
	}

	vector<token> tokenize(const char *source)
	{
	  lexer l(source);
	  vector<token> result;
	  while(l.next_is_not(token_kind::end_of_file))
	  {
	    result.push_back(l.look());
	    l.consume_token();
	  }
	  return result;
	}

	::testing::AssertionResult tok_is(const token &tok,
		                          token_kind kind)
	{
	  if(tok.get_kind() != kind)
	    return testing::AssertionFailure() << "Expected token kind " << kind
	      << ", got " << tok.get_kind();

	  return testing::AssertionSuccess();
	}

	::testing::AssertionResult tok_is(const token &tok,
		                          token_kind kind,
		                          unsigned line,
		                          unsigned column)
	{
	  if(tok.get_kind() != kind)
	    return testing::AssertionFailure() << "Expected token kind " << kind
	      << ", got " << tok.get_kind();

	  if(tok.get_location().get_line() != line)
	    return testing::AssertionFailure() << "Expected line " << line
	      << ", got " << tok.get_location().get_line();

	  if(tok.get_location().get_column() != column)
	    return testing::AssertionFailure() << "Expected column " << column
	      << ", got " << tok.get_location().get_column();

	  return testing::AssertionSuccess();
	}

} // unnamed namespace

TEST(Lexer, Comment)
{
  const char *sources[] = {
    "//", "// ", "// aaa"
  };
  for(size_t i = 0, e = array_lengthof(sources); i != e; i++) {
    vector<token> toks = tokenize(sources[i]);
    ASSERT_EQ(0U, toks.size());
  }
}

TEST(Lexer, TokAllParens)
{
  const char *source = "()[]{}";
  vector<token> toks = tokenize(source);
  ASSERT_EQ(6U, toks.size());
  ASSERT_TRUE(tok_is(toks[0], token_kind::lparen,   1, 1));
  ASSERT_TRUE(tok_is(toks[1], token_kind::rparen,   1, 2));
  ASSERT_TRUE(tok_is(toks[2], token_kind::lbracket, 1, 3));
  ASSERT_TRUE(tok_is(toks[3], token_kind::rbracket, 1, 4));
  ASSERT_TRUE(tok_is(toks[4], token_kind::lbrace,   1, 5));
  ASSERT_TRUE(tok_is(toks[5], token_kind::rbrace,   1, 6));
}

TEST(Lexer, TokSlash)
{
  const char *sources[] = {
    "/", "  /", " / ", "/\n", "// aaa\n/", "/// aaa\n/"
  };
  for(size_t i = 0, e = array_lengthof(sources); i != e; i++) {
    vector<token> toks = tokenize(sources[i]);
    ASSERT_EQ(1U, toks.size());
    ASSERT_TRUE(tok_is(toks[0], token_kind::slash));
  }
}


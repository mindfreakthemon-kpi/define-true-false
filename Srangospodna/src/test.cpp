#include <cstddef>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

#include "Lexer.h"
#include "Parser.h"

using lexer::Lexer;

namespace {

std::vector<Token> tokenize(std::string source) {
	Lexer l(source);
	Token* t = NULL;
	std::vector<Token> result;

	while ((t = l.scan())->getKind() != token::LT) {
		result.push_back(*t);
	}

	result.push_back(*t);

	return result;
}

node::Program *parse(std::vector<Token> toks, ErrorLoggerWrapper *eLW) {
	Parser p(toks, eLW);
	return p.parse();
}

::testing::AssertionResult parseResult_is(node::Program *program, CapturingErrorLogger *eL, std::string source) {
	if(program == NULL) {	
		LoggerRecord lR = eL->getRecords()[0];	
		return testing::AssertionFailure() << "AST is broken: "
			<< lR.message << " at " << lR.sl.getLine() << ":" << lR.sl.getColumn();
	}
	
	node::SyntaxDumper dumper;
	dumper.dump(program);
	
	if(source.compare(dumper.getResults()) != 0) {
		return testing::AssertionFailure() << "Source code and reconstructed code does not match!";
			//<< "Here is recostructed code: " << dumper.getResults() << "\n"
			//<< "Here is source code: " << dumper.getResults();
	}
	
	return testing::AssertionSuccess();
}

::testing::AssertionResult parseResult_is(node::Program *program, CapturingErrorLogger *eL, unsigned line, unsigned column) {
	if(program != NULL) {		
		return testing::AssertionFailure() << "AST is not broken!";
	}
	
	SourceLocation sl = eL->getRecords()[0].sl;
	
	if(sl.getColumn() != column || sl.getLine() != line) {
		return testing::AssertionFailure() << "Unexpected error! "
			<< "Expected at " << line << ":" << column << ". "
			<< "Got at " << sl.getLine() << ":" << sl.getColumn();
	}
	
	return testing::AssertionSuccess();
}

::testing::AssertionResult tok_is(const Token &tok, token::TokenKind kind) {
	if (tok.getKind() != kind)
		return testing::AssertionFailure() << "Expected token kind " << kind
				<< ", got " << tok.getKind();

	return testing::AssertionSuccess();
}

::testing::AssertionResult tok_is(const Token &tok, token::TokenKind kind,
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

}

TEST(Lexer, DefaultTest) {
	std::string source = "()[]{}==!~";
	std::vector<Token> toks = tokenize(source);
	ASSERT_EQ(10, toks.size());
	ASSERT_TRUE(tok_is(toks[0], token::LF_PARENTHESES, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], token::RT_PARENTHESES, 1, 2));
	ASSERT_TRUE(tok_is(toks[2], token::LF_SQ_BRACKET, 1, 3));
	ASSERT_TRUE(tok_is(toks[3], token::RT_SQ_BRACKET, 1, 4));
	ASSERT_TRUE(tok_is(toks[4], token::LF_CR_BRACKET, 1, 5));
	ASSERT_TRUE(tok_is(toks[5], token::RT_CR_BRACKET, 1, 6));
	ASSERT_TRUE(tok_is(toks[6], token::DEQUALS, 1, 7));
	ASSERT_TRUE(tok_is(toks[7], token::NEGATION, 1, 9));
	ASSERT_TRUE(tok_is(toks[8], token::TILDE, 1, 10));
	ASSERT_TRUE(tok_is(toks[9], token::LT, 1, 11));
}

TEST(Parser, DefaultTest) {
	std::string source = "function name(var1 : int, var2 : double, var3 : string) : void {"
"var i : int, v : double;"
"if(2==2) {"
"if(1) {"
"i = 2;"
"}"
"if(0) {"
"}"
"if(2*2*2*2) {"
"val[2+2*2] = func(call, 0, -5);"
"}"
"while(5&&5) {"
"}"
"}"
"while(!(!6)&&-5) {"
"}"
"}"
"function a() : void {"
"}";

	CapturingErrorLogger *eL = new CapturingErrorLogger();
	ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);

	std::vector<Token> tokens = tokenize(source);
	node::Program *program = parse(tokens, eLW);
	ASSERT_TRUE(parseResult_is(program, eL, source));
}

TEST(Parser, FailTest) {
	std::string source = "function () : void {}";
	
	CapturingErrorLogger *eL = new CapturingErrorLogger();
	ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);
	
	std::vector<Token> tokens = tokenize(source);
	node::Program *program = parse(tokens, eLW);
	ASSERT_TRUE(parseResult_is(program, eL, 1, 10));
}

TEST(Parser, WrongArrayAccess) {
	std::string source = "function name(var1 : int, var2 : double, var3 : string) : void {"
"var i : int, v : double;"
"if(2==2) {"
"if(1) {"
"i = 2;"
"}"
"if(0) {"
"}"
"if(2*2*2*2) {"
"val[2&&2] = func(call, 0, -5);" // Here is the error
"}"
"while(5&&5) {"
"}"
"}"
"while(!(!6)&&-5) {"
"}"
"}"
"function a() : void {"
"}";

	CapturingErrorLogger *eL = new CapturingErrorLogger();
	ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);

	std::vector<Token> tokens = tokenize(source);
	node::Program *program = parse(tokens, eLW);
	ASSERT_TRUE(parseResult_is(program, eL, 1, 138));
}

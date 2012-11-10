#include <cstddef>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

#include "Lexer.h"
#include "Parser.h"

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

::testing::AssertionResult parseResult_isPass(node::Program *program, CapturingErrorLogger *eL, std::string source) {
	if(program == NULL) {	
		LoggerRecord lR = eL->getRecords()[0];	
		return testing::AssertionFailure() << "AST is broken: "
			<< lR.message << " at " << lR.sl.getLine() << ":" << lR.sl.getColumn();
	}
	
	node::SyntaxDumper dumper;
	dumper.dump(program);
	
	if(source.compare(dumper.getResults()) != 0) {
		return testing::AssertionFailure() << "Source code and reconstructed code do not match!";
			//<< "Here is recostructed code: " << dumper.getResults() << "\n"
			//<< "Here is source code: " << dumper.getResults();
	}
	
	return testing::AssertionSuccess();
}

::testing::AssertionResult parseResult_isFail(node::Program *program, CapturingErrorLogger *eL, unsigned line, unsigned column) {
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
	std::string source =
		"function name(var1 : int[], var2 : double, var3 : string) : void {\n"
			"var2 = 2.12314;\n"
			"if(var2 >= 2.12){\n"
				"var3 = \"ololo\";"
			"};\n"
			"intval : int;\n"
			"intval = 23\n"
		"}";
	std::vector<Token> toks = tokenize(source);
	ASSERT_TRUE(tok_is(toks[0], token::FUNCTION, 1, 1));
	ASSERT_TRUE(tok_is(toks[1], token::ID, 1, 10));
	ASSERT_TRUE(tok_is(toks[2], token::LF_PARENTHESES, 1, 14));
	ASSERT_TRUE(tok_is(toks[3], token::ID, 1, 15));
	ASSERT_TRUE(tok_is(toks[4], token::COLON, 1, 20));
	ASSERT_TRUE(tok_is(toks[5], token::TYPE, 1, 22));
	ASSERT_TRUE(tok_is(toks[6], token::LF_SQ_BRACKET, 1, 25));
	ASSERT_TRUE(tok_is(toks[7], token::RT_SQ_BRACKET, 1, 26));
	ASSERT_TRUE(tok_is(toks[8], token::COMMA, 1, 27));
	ASSERT_TRUE(tok_is(toks[9], token::ID, 1, 29));
	ASSERT_TRUE(tok_is(toks[10], token::COLON, 1, 34));
	ASSERT_TRUE(tok_is(toks[11], token::TYPE, 1, 36));
	ASSERT_TRUE(tok_is(toks[12], token::COMMA, 1, 42));
	ASSERT_TRUE(tok_is(toks[13], token::ID, 1, 44));
	ASSERT_TRUE(tok_is(toks[14], token::COLON, 1, 49));
	ASSERT_TRUE(tok_is(toks[15], token::TYPE, 1, 51));
	ASSERT_TRUE(tok_is(toks[16], token::RT_PARENTHESES, 1, 57));
	ASSERT_TRUE(tok_is(toks[17], token::COLON, 1, 59));
	ASSERT_TRUE(tok_is(toks[18], token::TYPE, 1, 61));
	ASSERT_TRUE(tok_is(toks[19], token::LF_CR_BRACKET, 1, 66));
	ASSERT_TRUE(tok_is(toks[20], token::ID, 2, 1));
	ASSERT_TRUE(tok_is(toks[21], token::EQUALS, 2, 6));
	ASSERT_TRUE(tok_is(toks[22], token::DOUBLE_LITERAL, 2, 8));
	ASSERT_TRUE(tok_is(toks[23], token::SEMICOLON, 2, 15));
	ASSERT_TRUE(tok_is(toks[24], token::IF, 3, 1));
	ASSERT_TRUE(tok_is(toks[25], token::LF_PARENTHESES, 3, 3));
	ASSERT_TRUE(tok_is(toks[26], token::ID, 3, 4));
	ASSERT_TRUE(tok_is(toks[27], token::MORE_EQUALS, 3, 9));
	ASSERT_TRUE(tok_is(toks[28], token::DOUBLE_LITERAL, 3, 12));
	ASSERT_TRUE(tok_is(toks[29], token::RT_PARENTHESES, 3, 16));
	ASSERT_TRUE(tok_is(toks[30], token::LF_CR_BRACKET, 3, 17));
	ASSERT_TRUE(tok_is(toks[31], token::ID, 4, 1));
	ASSERT_TRUE(tok_is(toks[32], token::EQUALS, 4, 6));
	ASSERT_TRUE(tok_is(toks[33], token::STRING_LITERAL, 4, 8));
	ASSERT_TRUE(tok_is(toks[34], token::SEMICOLON, 4, 15));
	ASSERT_TRUE(tok_is(toks[35], token::RT_CR_BRACKET, 4, 16));
	ASSERT_TRUE(tok_is(toks[36], token::SEMICOLON, 4, 17));
	ASSERT_TRUE(tok_is(toks[37], token::ID, 5, 1));
	ASSERT_TRUE(tok_is(toks[38], token::COLON, 5, 8));
	ASSERT_TRUE(tok_is(toks[39], token::TYPE, 5, 10));
	ASSERT_TRUE(tok_is(toks[40], token::SEMICOLON, 5, 13));
	ASSERT_TRUE(tok_is(toks[41], token::ID, 6, 1));
	ASSERT_TRUE(tok_is(toks[42], token::EQUALS, 6, 8));
	ASSERT_TRUE(tok_is(toks[43], token::INT_LITERAL, 6, 10));
	ASSERT_TRUE(tok_is(toks[44], token::RT_CR_BRACKET, 7, 1));
	ASSERT_TRUE(tok_is(toks[45], token::LT, 7, 2));
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
	ASSERT_TRUE(parseResult_isPass(program, eL, source));
}

TEST(Parser, FailTest) {
	std::string source = "function () : void {}";
	
	CapturingErrorLogger *eL = new CapturingErrorLogger();
	ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);
	
	std::vector<Token> tokens = tokenize(source);
	node::Program *program = parse(tokens, eLW);
	ASSERT_TRUE(parseResult_isFail(program, eL, 1, 10));
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
	ASSERT_TRUE(parseResult_isFail(program, eL, 1, 138));
}

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Lexer.h"

using namespace std;
using namespace token;

int main(int argc, char** argv) {

	Lexer lexer(
			"function name(var1 : int[], var2 : double, var3 : string) : void {\n"
				"var2 = 2.12314;\n"
				"if(var2 >= 2.12){\n"
					"var3 = \"ololo\";"
				"};\n"
				"intval : int;\n"
				"intval = 23\n"
			"}");
	Token * tok = NULL;
	int i = 0;
	do {
		tok = lexer.scan();
		if (tok != NULL) {
			//ASSERT_TRUE(tok_is(toks[0], token::LF_PARENTHESES, 1, 1));
			cout << "ASSERT_TRUE(tok_is(toks[" << i << "], token::" << token::getKindString(tok->getKind()) << ", " << tok->getLocation().row << ", " << tok->getLocation().col << "));" << endl;
			//cout << "Token:" << getKindString(tok->getKind()) << " with position:"
					//<< tok->getLocation().toString() << endl;
		}
		i++;
	} while ((tok == NULL) || tok->getKind() != LT);

	//cin.get();
	return 0;
}

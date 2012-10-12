/* 
 * File:   main.cpp
 * Author: mindfreakthemon
 *
 * Created on September 26, 2012, 9:45 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
//#include <pcrecpp.h>
#include <string>
#include <vector>
#include <Token.h>
#include <Lexer.h>
using namespace std;
using namespace token;
/*
 * 
 */
int main(int argc, char** argv) {
	if (argc <= 1) {
        cout << "Usage: " << argv[0] << " <Filename>" << endl;
		cin.get();
        exit(1);
    }
	
	char *filename = argv[1];
	
	string file(filename);

	//TODO rework this peace of code
	//vector<Token*> tokens;
	Lexer lexer(file);
	Token * tok = NULL;
	do{
		tok = lexer.scan();
		if(tok != NULL){

			cout << "Token:" << tok->getKind() << " with position:" << tok->_loc.toString() << endl;
		}
	}while((tok == NULL) || tok->getKind() != LT);
	
	cin.get();
	return 0;
}


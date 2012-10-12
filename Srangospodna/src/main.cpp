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
#include "Tag.h"
using namespace std;

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
	
	string line,text;
	ifstream file (filename);
	
	while ( file.good() ) {
		if (file.is_open()) {
			while (file.good()) {
				getline(file, line);
				text.append(line);
				text.append("\n");
				//cout << line << endl;
			}
			file.close();
			cout << "readed file:" << endl << text;
		}
	}
	//TODO rework this peace of code
	vector<Token*> tokens;
	Lexer lexer(text);
	for(int i = 0; i<=100; i++) {
		//tokens.push_back(lexer.scan());
		Token * tok = lexer.scan();
		if(tok != NULL){
			cout << "Token:" << tok->getKind() << " that represents char:" << (char)tok->getKind() << endl;
			if(tok->getKind() == Tag::STRING){
				cout << ((StringToken*)tok)->getData() << endl;
			}
		}
	}
	
	cin.get();
	return 0;
}


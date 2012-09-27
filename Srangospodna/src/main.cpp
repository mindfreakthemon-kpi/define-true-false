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
#include <Token.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	if (argc <= 1) {
        cout << "Usage: " << argv[0] << " <Filename>" << endl;
        exit(1);
    }
	
	char *filename = argv[1];
	
	string line;
	ifstream file (filename);
	
	while ( file.good() ) {
		if (file.is_open()) {
			while (file.good()) {
				getline(file, line);
				cout << line << endl;
			}
			file.close();
		}
	}	
			
	
	return 0;
}


/* 
 * File:   Token.h
 * Author: mindfreakthemon
 *
 * Created on September 27, 2012, 3:44 PM
 */

#ifndef TOKEN_H
#define	TOKEN_H

class Token {
public:	
	Token(int type, int col, int row);
	virtual ~Token();
	
	int getType();
private:
	int type;
	int col;
	int row;
};

#endif	/* TOKEN_H */


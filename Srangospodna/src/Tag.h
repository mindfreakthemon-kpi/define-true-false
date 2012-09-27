/* 
 * File:   Tag.h
 * Author: mindfreakthemon
 *
 * Created on September 27, 2012, 4:01 PM
 */

#ifndef TAG_H
#define	TAG_H

class Tag {
public:
	static const int ID = 256;
	static const int INT = 257;
	static const int DOUBLE = 258;
	static const int STRING = 259;
	static const int BOOL = 260;
	
	static const int OPERATOR = 261;
	static const int COMMENT = 262;
	
	static const int TYPE_SCALAR = 263;
	static const int TYPE_ARRAY = 264;	
};

#endif	/* TAG_H */


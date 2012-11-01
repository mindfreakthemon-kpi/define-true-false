#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

#include "Token.h"

class ErrorLogger {
public:
	virtual void error(SourceLocation sl, const std::string &message) = 0;
	virtual void warning(SourceLocation sl, const std::string &message) = 0;

	virtual ~ErrorLogger() {};
};

class StdoutErrorLogger : public ErrorLogger {
public:
	void error(SourceLocation sl, const std::string &message) {
		std::cout << "Error (" << sl.getLine() << ":" << sl.getColumn() << ")! " << message << std::endl;
	}
	
	void warning(SourceLocation sl, const std::string &message) {
		std::cout << "Warning (" << sl.getLine() << ":" << sl.getColumn() << ")! " << message << std::endl;
	}
};

/* Pure magic */
class ErrorLoggerWrapper {
public:
	ErrorLoggerWrapper(ErrorLogger *eL) :
 		eL(eL) {}
	
private:
	void append(std::ostream &) {}
	
	template<typename T1, typename... Ts>
	void append(std::ostream &out, T1 &&message_first, Ts &&... message_rest)
	{
		out << message_first;
		append(out, std::forward<Ts>(message_rest)...);
	}
	
public:
	template<typename T1, typename... Ts>
 	void error(SourceLocation sl, T1 &&message_first, Ts &&... message_rest) {
		std::ostringstream ss;
		append(ss, message_first, message_rest...);
		eL->error(sl, ss.str());
	}
	
private:
	ErrorLogger *eL;
};

#endif /* ERROR_H */

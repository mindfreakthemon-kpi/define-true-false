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
	virtual void notice(SourceLocation sl, const std::string &message) = 0;

	virtual ~ErrorLogger() {};
};

class StdoutErrorLogger : public ErrorLogger {
public:
	void error(SourceLocation sl, const std::string &message) {
		output("Error", sl, message);
	}
	
	void warning(SourceLocation sl, const std::string &message) {
		output("Warning", sl, message);
	}
	
	void notice(SourceLocation sl, const std::string &message) {
		output("Notice", sl, message);
	}
private:
	void output(std::string type, SourceLocation sl, const std::string &message) {
		std::cout << type << " (" << sl.getLine() << ":" << sl.getColumn() << ")! " << message << std::endl;
	}
};

struct LoggerRecord {
	enum RecordLevel {
		NOTICE = 1,
		WARNING,
		ERROR
	};
		
	RecordLevel level;
	SourceLocation sl;
	std::string message;
};

class CapturingErrorLogger : public ErrorLogger {
public:	
	void error(SourceLocation sl, const std::string &message) {
		save(LoggerRecord::ERROR, sl, message);
	}
	
	void warning(SourceLocation sl, const std::string &message) {
		save(LoggerRecord::WARNING, sl, message);
	}
	
	void notice(SourceLocation sl, const std::string &message) {
		save(LoggerRecord::NOTICE, sl, message);
	}
	
	const std::vector<LoggerRecord> &getRecords() {
		return records;
	}
	
private:
	void save(LoggerRecord::RecordLevel level, SourceLocation sl, const std::string &message) {
		LoggerRecord lR;
		lR.level = level;
		lR.sl = sl;
		lR.message = message;
		records.push_back(lR);
	}
	
	std::vector<LoggerRecord> records;
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
	
	template<typename T1, typename... Ts>
 	void warning(SourceLocation sl, T1 &&message_first, Ts &&... message_rest) {
		std::ostringstream ss;
		append(ss, message_first, message_rest...);
		eL->warning(sl, ss.str());
	}
	
	ErrorLogger *getErrorLogger() const {
		return eL;
	}
	
private:
	ErrorLogger *eL;
};

#endif /* ERROR_H */

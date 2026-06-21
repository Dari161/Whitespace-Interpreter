#ifndef READER_H
#define READER_H

#include <string>

class Reader {
private:
	std::string input;
	size_t pos;
	const int hexCharToInt(const char hexChar) const;

public:
	Reader() = default;

	Reader(const std::string& str) : input(str), pos(0) {}

	void setInput(const std::string& str) {
		input = str;
		pos = 0;
	}

	const int readNum();
	const char readChar();
};

#endif // READER_H

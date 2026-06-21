#ifndef READER_H
#define READER_H

#include <string>

class Reader {
private:
	std::string input;
	size_t pos;
	int hexCharToInt(const char hexChar) const;

public:
	Reader() = default;

	Reader(const std::string& str) : input(str), pos(0) {}

	void setInput(const std::string& str) {
		input = str;
		pos = 0;
	}

	int readNum();
	char readChar();
};

#endif // READER_H

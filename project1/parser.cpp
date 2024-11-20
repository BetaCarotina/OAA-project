#include <iostream>
#include <string>

class Token {
public:
	std::string type;
	std::string value;
	Token(std::string t, std::string v) {
		type = t;
		value = v;
	}
};

class Lexer {
	std::string text;
	int pos = 0;
	char currchar;

public:
	Lexer(std::string t) {
		text = t;
		currchar = text[0];
	}

	void gettext() {
		std::cout << text << std::endl;
	}


};
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "database.cpp"
#include <regex>

class Token {
public:
	std::string type;
	std::string value;
	Token(std::string t, std::string v) {
		type = t;
		value = v;
	}

	Token() {
		type = "unknown";
		value = "";
	}
};

class Parser {
	std::string text;
	int pos = 0;
	char currchar;
	char whitespaces[5] = { ' ', '\t', '\r', '\n', '\0'};

	Database database;

public:
	Parser() {
		text = " ";
		currchar = ' ';
	}

	void getinput() {
		std::cout << "...";
		std::cin >> text;
		pos = 0;
		currchar = text[pos];
	}

	void advance() {
		pos++;
		if (pos > text.length()) {
			getinput();
		}
		else {
			currchar = text[pos];
		}
	}

	void printtext() {
		std::cout << text << std::endl;
	}

	void skipWhitespace() {
		char* ch = std::find(std::begin(whitespaces), std::end(whitespaces), currchar);
		while (ch != std::end(whitespaces)) {
			advance();
			ch = std::find(std::begin(whitespaces), std::end(whitespaces), currchar);
		}
	}

	Token getNextToken() {
		Token t("unknown", "");
		char* ch = std::find(std::begin(whitespaces), std::end(whitespaces), currchar);
		if (ch != std::end(whitespaces)) {
			skipWhitespace();
		}
		if (currchar == '"') {
			t.type = "string";
			std::cout << t.type;
			advance();
			while (currchar != '"') {
				t.value += currchar;
				advance();
			}
			advance();
			return t;
		}
		else if (currchar == '>' || currchar == '=' || currchar == '<') {
			t.type = "sign";
			//std::cout << t.type;
			t.value = currchar;
			advance();
			return t;
		}
		else if (currchar == ',') {
			t.type = "comma";
			//std::cout << t.type;
			t.value = currchar;
			advance();
			return t;
		}
		else if (currchar == ';') {
			t.type = "end";
			//std::cout << t.type;
			t.value = currchar;
			advance();
			return t;
		}
		else if (currchar == '(' || currchar == ')') {
			t.type = "bracket";
			//std::cout << t.type;
			t.value = currchar;
			advance();
			return t;
		}
		else {
			t.type = "keyword";
			t.value += currchar;
			advance();
			char* ch = std::find(std::begin(whitespaces), std::end(whitespaces), currchar);
			while (ch == std::end(whitespaces) && currchar != ',' && currchar != ';' && currchar != '(' && currchar!=')') {
				//std::cout << (ch == std::end(whitespaces));
				//std::cout << t.value;
				if (currchar == '"') {
					throw std::invalid_argument("Unclear usage of quotes in the middle of a name");
				}
				t.value += currchar;
				advance();
				ch = std::find(std::begin(whitespaces), std::end(whitespaces), currchar);
				//std::cout << currchar << (int) currchar << std::endl;
			}
			return t;
		}
	}

	void tokenize() { //test
		getinput();
		while (true) {
			Token t = getNextToken();
			std::cout << t.value << std::endl;
		}
	}

	void start() {
		getinput();
		if (text == "q") {
			return;
		}
		Token t = getNextToken();
		if (std::regex_match(t.value, std::regex("create", std::regex_constants::icase))) {
			//create
			Token table = getNextToken();
			if (!std::regex_match(table.value, std::regex("[a-zA-Z][a-zA-Z0-9_]*"))){
				std::cerr << "Invalid table name: " << table.value;
			}
			Token bracket = getNextToken();
			if (bracket.value != "(") {
				std::cerr << "Unexpected argument " << table.value << ": expected a list of column names in brackets";
			}
			Token arg = getNextToken();
			std::vector<std::string> columns;
			while (arg.value != ")") {
				if (arg.type != "keyword" && arg.type != "string") {
					std::cerr << "Unexpected argument " << arg.value;
					return;
				}
				std::cout << arg.value << std::endl;
				columns.push_back(arg.value);
				Token sep = getNextToken();
				if (sep.type == "comma") {
					arg = getNextToken();
				}
				else if (sep.value == ")") {
					break;
				}
				else {
					std::cerr << "Unexpected argument " << sep.value << "; Arguments must be separated by commas";
					return;
				}
			}
			for (int i = 0; i < columns.size(); i++) {
				std::cout << columns[i];
			}
			database.createTable(table.value, columns);
			std::cout << "Created the table " << table.value << std::endl;
			std::cout << "here are the columns: ";
			database.printTableCols(table.value);
			std::cout << std::endl;

		}
		else if (std::regex_match(t.value, std::regex("insert", std::regex_constants::icase))) {
			//insert
		}
		else if (std::regex_match(t.value, std::regex("select", std::regex_constants::icase))) {
			//select
		}
		else {
			std::cout << "Unknown command: " << t.value;
		}
		start();

	}
};
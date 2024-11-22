#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

class Table {
	std::vector<std::string> columns; //names
	std::vector<std::vector<std::string>> rows;

public:
	Table(std::vector<std::string> cols) {
		columns = cols;
	}

	void printCols() {
		for (int i = 0; i < columns.size(); i++) {
			std::cout << columns[i] << "|";
		}
	}
};

class Database {
	std::unordered_map<std::string, Table> tables;

public:
	void createTable(std::string name, std::vector<std::string> cols) {
		Table newtable(cols);
		tables.insert(std::make_pair(name, newtable));
	}

	void printTableCols(std::string name) {
		tables.at(name).printCols();
	}
};
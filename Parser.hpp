#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Parser {
	string s;

	bool IsLetter(char c); // функция проверка на букву
	bool IsDigit(char c); //функция проверка на цифру


public:
	Parser(string s); // конструктор
	vector<string> Parse(); // метод разделения строк на лексемы
};

Parser::Parser(string s) {
	this->s = s;
}

bool Parser::IsLetter(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Parser::IsDigit(char c) {
	return c >= '0' && c <= '9';
}

vector<string> Parser::Parse() {
    vector<string> lexemes;
	int i = 0;

	while (i < s.length()) {
		if ((s[i] == '(') || (s[i] == ')') || (s[i] == '+') || (s[i] == '-') || (s[i] == '/') || (s[i] == '*') || (s[i] == '^') || (s[i] == '=')) {
		    lexemes.push_back(string(1, s[i]));
		    i++;
		}
		else if (IsLetter(s[i])) {
			string word = "";

			while (i < s.length() && (IsLetter(s[i]) || IsDigit(s[i]))) {
				word += s[i];
				i++;
			}

			lexemes.push_back(word);
		}
		else if (IsDigit(s[i])) {
			string num = "";
			int k = 0;

			while (i < s.length() && (s[i] == '.' || IsDigit(s[i]))) {
				if (s[i] == '.') {
					k++;
				}

				if (k > 1) {
					throw string("incorrect number");
				}

				num += s[i];
				i++;
			}

			lexemes.push_back(num);
		}
		else if (s[i] == ' ') {
			i++;
		}
		else {
			throw string("unknown character in expression '") + s[i] + "'";
		}	
	}

	if (lexemes.size() == 0) {
		throw string("expression is empty");
	}

	return lexemes;
}

#include <iostream>
#include <string>
#include <cmath>
#include "Calculator.hpp"
using namespace std;

int main() {
	Calculator calculator;
	string s;

	while (true) {
		cout << ">";
		getline(cin, s);

		if (s == "quit") {
			break;
		}

		calculator.ProcessCommand(s);
	}
}

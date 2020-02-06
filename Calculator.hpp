#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "Parser.hpp"
#include "Stack.hpp"

using namespace std;

class Calculator {
	struct Variable {
		string name;
		double value;
	};

	struct UserFunction {
		string name;
		string arg;
		vector<string> rpn;
	};

	vector<Variable> variables; //вектор переменных с именем variables
	vector<UserFunction> userFunctions; // вектор пользовательских функций
	string mode;

	int GetVariableIndex(string name);
	void AddVariable(string name, double value);

	int GetUserFunctionIndex(string name);

	bool IsNumber(string s);
	bool IsFunction(string s);
	bool IsUserFunction(string s);
	bool IsOperator(string s);
	bool IsIdentifier(string s);
	bool IsVariable(string s);

	double CalculateOperator(string op, double arg1, double arg2); // вычисляет операторы
	double CalculateFunction(string name, double arg); // вычисляет функции
	double CalculateUserFunction(string name, double arg); // вычисляет пользовательские функции
	double Calculate(const vector<string> &rpn, int start = 0); // вычисляет значение выражения в польской записи

	void ProcessSet(vector<string> lexemes);
	void ProcessMode(vector<string> lexemes);
	void ProcessState();

	void ProcessDef(vector<string> lexemes);

	int GetPriority(string lexeme);
	vector<string> GetRPN(vector<string> lexemes, int start = 0); // метод перевода в польскую запись

public:
	Calculator();
	void ProcessCommand(string s);
};

Calculator::Calculator() {
	mode = "rad";
}

int Calculator::GetVariableIndex(string name) {
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].name == name) {
			return i;
		} 
	}
	return -1;
}

void Calculator::AddVariable(string name, double value) {
	int index = GetVariableIndex(name);
	if (index != -1) {
		variables[index].value = value;
	}
	else
		variables.push_back({name, value}); //добавляем переменную в векnор
}

int Calculator::GetUserFunctionIndex(string name) {
	for (int i = 0; i < userFunctions.size(); i++) {
		if (userFunctions[i].name == name) {
			return i;
		} 
	}
	return -1;
}

bool Calculator::IsNumber(string s) {
	for (int i = 0; i < s.length(); i++) {
		if ((s[i] < '0' || s[i] > '9') && s[i] != '.') {
			return false;
		}
	}
	return true;
}

bool Calculator::IsFunction(string s) {
	return (s == "sin" || s == "cos" || s == "tan" || s == "tg" || s == "cot" ||s == "ctg" || s == "arcsin" || s == "arccos" || s == "arctg" || s == "arcctg" || s == "sqrt" || s == "log" || s == "ln" || s == "lg");
}

bool Calculator::IsUserFunction(string s) {
	return GetUserFunctionIndex(s) != -1;
}

bool Calculator::IsOperator(string s) {
	return ((s == "+") || (s == "-") || (s == "/") || (s == "*") || (s == "^"));
}

bool Calculator::IsIdentifier(string s) {
	if (IsFunction(s) || IsNumber(s)) {
		return false;
	}
	for (int i = 0; i < s.length(); i++) {
		if (s[i] >= '0' && s[i] <= '9') 
			continue;

		if (s[i] >= 'a' && s[i] <= 'z') 
			continue;

		if (s[i] >= 'A' && s[i] <= 'Z') 
			continue;

		return false;
	}
	return true;
}

bool Calculator::IsVariable(string s) {
	return GetVariableIndex(s) != -1;
}

double Calculator::CalculateOperator(string op, double arg1, double arg2) {
	if (op == "+") {
		return arg1 + arg2;
	}
	if (op == "-") {
		return arg1 - arg2;
	}
	if (op == "*") {
		return arg1 * arg2;
	}
	if (op == "/") {
		return arg1 / arg2;
	}
	if (op == "^") {
		return pow(arg1, arg2);
	}
	throw string("Unknown operation '") + op + "'";
}

double Calculator::CalculateFunction(string name, double arg) {
	if (mode == "deg" && (name == "sin" || name == "cos" || name == "tan" || name == "tg" || name == "ctg" || name == "cot" || name == "arcsin" || name == "arccos" || name == "arctg" || name == "arcctg")) {
		arg = arg * M_PI / 180;
	}
	if (name == "sin") {
		return sin(arg);
	}
	if (name == "cos") {
		return cos(arg);
	}
	if (name == "tan") {
		return tan(arg);
	}
	if (name == "tg") {
		return tan(arg);
	}
	if (name == "ctg") {
		return 1 / tan(arg);
	}
	if (name == "cot") {
		return 1 / tan(arg);
	}
	if (name == "arcsin") {
		return asin(arg);
	}
	if (name == "arccos") {
		return acos(arg);
	}
	if (name == "arctg") {
		return atan(arg);
	}
	if (name == "arcctg") {
		return (M_PI)/2 - atan(arg);
	}
	if (name == "sqrt") {
		return sqrt(arg);
	}
	if (name == "log") {
		return log(arg);
	}
	if (name == "ln") {
		return log(arg);
	}
	if (name == "lg") {
		return log10(arg);
	}
	throw string("Unknown function '") + name + "'";
}

double Calculator::CalculateUserFunction(string name, double arg) {
	int index = GetUserFunctionIndex(name);
	UserFunction func = userFunctions[index];
	vector<string> rpn = func.rpn;
	for (int i = 0; i < rpn.size(); i++) {
		if (rpn[i] == func.arg) {
			rpn[i] = to_string(arg);
		}
	}
	return Calculate(rpn);
}

// вычисляет значение выражения в польской записи
double Calculator::Calculate(const vector<string> &rpn, int start) {
	Stack<double> stack;

	// cout << "rpn: ";
	// for (int i = 0; i < rpn.size(); i++)
	// 	cout << "[" << rpn[i] << "]  ";

	for (int i = start; i < rpn.size(); i++){
		if (IsNumber(rpn[i])) {
			double res = stod(rpn[i]);
			stack.Push(res);
		}
		else if (IsVariable(rpn[i])) {
			int index = GetVariableIndex(rpn[i]);
			stack.Push(variables[index].value);
		}
		else if (IsFunction(rpn[i])) {
			if (stack.GetSize() < 1) {
				throw string("Unable to calculate function '") + rpn[i] + "'";
			}

			double arg = stack.Top();
			stack.Pop();
			double res = CalculateFunction(rpn[i], arg);
			stack.Push(res);
		}
		else if (IsUserFunction(rpn[i])) {
			if (stack.GetSize() < 1) {
				throw string("Unable to calculate function '") + rpn[i] + "'";
			}

			double arg = stack.Top();
			stack.Pop();
			double res = CalculateUserFunction(rpn[i], arg);
			stack.Push(res);
		}
		else if (IsOperator(rpn[i])) {
			if (stack.GetSize() < 2) {
				throw string("Unable to calculate operator '") + rpn[i] + "'";
			}

			double arg2 = stack.Top();
			stack.Pop();
			double arg1 = stack.Top();
			stack.Pop();
			double res = CalculateOperator(rpn[i], arg1, arg2);
			stack.Push(res);
		}
		else if (rpn[i] == "!") {
			if (stack.IsEmpty()) {
				throw string("Unable to calculate unary minus");
			}
			double arg = stack.Top();
			stack.Pop();
			stack.Push(-arg);
		}
		else {
			throw string("Unknown lexeme '") + rpn[i] + "'";
		}
	}

	if (stack.GetSize() != 1) {
		throw string("incorrect expression");
	}
	return stack.Top();
}

void Calculator::ProcessSet(vector<string> lexemes) {
	if (lexemes.size() < 4 || lexemes[2] != "=") {
		throw string("incorrect set command");
	}
	if (!IsIdentifier(lexemes[1])) {
		throw string("'") + lexemes[1] + "' is not correct name";
	}

	double value = Calculate(lexemes, 3);
	AddVariable(lexemes[1], value);
}

void Calculator::ProcessMode(vector<string> lexemes) {
	if (lexemes.size() < 2) {
		cout << mode << endl;
	}
	else if (lexemes[1] == "rad") {
		mode = "rad";
	}
	else if (lexemes[1] == "deg") {
		mode = "deg";
	}
	else {
		throw string("incorrect mode command");
	}
}

void Calculator::ProcessState() {
	if (variables.size() != 0) {
		cout << "+------------+-------------+" << endl;
		cout << "|    name    |    value    |" << endl;
		cout << "+------------+-------------+" << endl;

		for (int i = 0; i < variables.size(); i++) {
			cout << "|" << setw(11) << variables[i].name << " |" << setw(12) << variables[i].value << " |" << endl;
		}

		cout << "+------------+-------------+" << endl;
	}
	else { 
		cout << "no variables" << endl;
	}
}

void Calculator::ProcessDef(vector<string> lexemes) {
	if (lexemes.size() < 7)
		throw string("Incorrect function definition");

	if (!IsIdentifier(lexemes[1]) && !IsUserFunction(lexemes[1])) {
		throw string("Incorrect function name");
	}

	for (int i = 0; i < variables.size(); i++) {
		if (lexemes[1] == variables[i].name) {
			throw string("Incorrect function name");
		}
	}

	if (!IsIdentifier(lexemes[3])) {
		throw string("Incorrect argument in function definition");
	}

	if (lexemes[2] != "(" || lexemes[4] != ")" || lexemes[5] != "=")
		throw string("Incorrect function definition");

	for (int i = 6; i < lexemes.size(); i++) {
		if (IsIdentifier(lexemes[i]) && lexemes[i] != lexemes[3]) {
			throw string("Incorrect function definition");
		}
	}

	UserFunction func;
	func.name = lexemes[1];
	func.arg = lexemes[3];
	func.rpn = GetRPN(lexemes, 6);

	userFunctions.push_back(func);
}

void Calculator::ProcessCommand(string s) {
	try {
		Parser parser(s);
		vector<string> lexemes = parser.Parse();

		if (lexemes[0] == "set") {
			ProcessSet(lexemes);
		}
		else if (lexemes[0] == "mode") {
			ProcessMode(lexemes);
		}
		else if (lexemes[0] == "state") {
			ProcessState();
		}
		else if (lexemes[0] == "def") {
			ProcessDef(lexemes);
		}
		else {
			vector<string> rpn = GetRPN(lexemes);
			cout << Calculate(rpn) << endl;
		}
	}
	catch (string error) {
		cout << error << endl;
	}
}

int Calculator::GetPriority(string lexeme) {
	if (lexeme == "^")
		return 4;

	if (lexeme == "!")
		return 3;

	if (lexeme == "*" || lexeme == "/") 
		return 2;

	if (lexeme == "+" || lexeme == "-") 
		return 1;

	return 0;
}

vector<string> Calculator::GetRPN(vector<string> lexemes, int start) {
	vector<string> rpn;
	Stack<string> stack;
	bool isUnary = true;

	for (int i = start; i < lexemes.size(); i++) {
		if (IsFunction(lexemes[i]) || IsUserFunction(lexemes[i])) {
			stack.Push(lexemes[i]);
			isUnary = true;
		}
		else if (IsNumber(lexemes[i]) || IsIdentifier(lexemes[i])) {
			rpn.push_back(lexemes[i]);
			isUnary = false;
		}
		else if (lexemes[i] == "(") {
			stack.Push(lexemes[i]);
			isUnary = true;
		}
		else if (lexemes[i] == ")") {
			while (!stack.IsEmpty() && stack.Top() != "(") {
				rpn.push_back(stack.Top());
				stack.Pop();
			}

			if (stack.IsEmpty()) {
				throw string("Incorrect expression");
			}

			stack.Pop();
			isUnary = false;
		}
		else if (IsOperator(lexemes[i])) {
			string op = lexemes[i];

			if (op == "-" && isUnary)
				op = "!";

			while (!stack.IsEmpty() && (GetPriority(stack.Top()) >= GetPriority(op) || IsFunction(stack.Top()) || IsUserFunction(stack.Top()))) {
				rpn.push_back(stack.Top());
				stack.Pop();
			}

			stack.Push(op);
			isUnary = false;
		}
		else 
			throw string("Incorrect expression");
	}

	while (!stack.IsEmpty()) {
		if (!IsFunction(stack.Top()) && !IsUserFunction(stack.Top()) && !IsOperator(stack.Top()) && stack.Top() != "!") {
			throw string("Incorrect expression");
		}

		rpn.push_back(stack.Top());
		stack.Pop();
	}

	return rpn;
}

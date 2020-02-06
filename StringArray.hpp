#pragma once

#include <iostream>
#include <string>

using namespace std;

class StringArray {
	int n;
    string *values;

public:
	StringArray();
	
	int GetSize();
	void Add(string a);
	string operator[](int index);
	
	~StringArray();
};

StringArray::StringArray() {
	this->n = 0;
	values = NULL;
}


int StringArray::GetSize() {
	return n;
}

void StringArray::Add(string a) {
	string *tmp = new string[n + 1];
	for (int i = 0; i < n; i++) {
		tmp[i] = values[i];
	}

	tmp[n] = a;
	n++;
	delete[] values;
	values = tmp;
}

string StringArray::operator[](int index) {
	return values[index];
}

StringArray::~StringArray() {
	delete[] values;
}

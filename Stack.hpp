#pragma once

#include <iostream>  

using namespace std;

template <typename T>
class Stack {
	struct Node {
		T value; // значение элемента стека 
		Node *next; // указатель на следующий элемент
	};
	Node *top; // указатель на первый элемент 
	int size; // количество элементов в стеке

public: 
	Stack();

	void Push(T value); // добавление элемента в стек
	T Top(); // возвращаем первый элемент из стека
	bool IsEmpty(); // проверяем стек на пустоту
	void Pop(); // удаляет первый элемент из стека
	void Print();
	int GetSize(); // возвращает количество элементов в стеке
};

template <typename T>
Stack<T>::Stack() {
	top = NULL;
	size = 0;
}

// добавление элемента в стек на первое место
template <typename T>
void Stack<T>::Push(T value) {
	Node *node = new Node; // выделяем память под элемент
	node->value = value; // добавляем новый элемент
	node->next = top;
	top = node; // присваиваем указатель 
	size++;
} 

template <typename T>
T Stack<T>::Top() {
	return top->value;
}

template <typename T>
bool Stack<T>::IsEmpty() {
	return top == NULL;
}

template <typename T>
void Stack<T>::Pop() {
	Node *next = top->next; // запоминаем указатель след элемента
	delete top;
	top = next;
	size--;
}

template <typename T>
void Stack<T>::Print() {
	Node *node = top;
	while (node) {
		cout << node->value << "  ";
		node = node->next;
	}
	cout << endl;
}

template <typename T>
int Stack<T>::GetSize() {
	return size;
}

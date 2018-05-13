#pragma once

#include <iostream>
using namespace std;

template <typename T, typename S>
class Tuple {
public:
	T first;
	S second;

	Tuple(const T first, const S second);
	void print();
};

template <typename T, typename S>
Tuple<T, S>::Tuple(const T first, const S second) {
	this->first = first;
	this->second = second;
}

template <typename T, typename S>
void Tuple<T, S>::print() {
	cout << "(" << first << ", " << second << ")" << endl;
}
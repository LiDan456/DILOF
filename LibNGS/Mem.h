#pragma once

using namespace std;

class Mem {
public:
	template <typename T> static void alloc2D(T**& dest, const int NUM_ROWS, const int NUM_COLS);
	template <typename T> static T** alloc2D(const int NUM_ROWS, const int NUM_COLS);
	template <typename T> static void delete2D(T** arr, const int NUM_ROWS);
};

template <typename T>
static void Mem::alloc2D(T**& dest, const int NUM_ROWS, const int NUM_COLS) {
	dest = new T*[NUM_ROWS];

	for (int i = 0; i < NUM_ROWS; i++) {
		dest[i] = new T[NUM_COLS];
	}
}

template <typename T>
static T** Mem::alloc2D(const int NUM_ROWS, const int NUM_COLS) {
	T** dest = new T*[NUM_ROWS];

	for (int i = 0; i < NUM_ROWS; i++) {
		dest[i] = new T[NUM_COLS];
	}

	return dest;
}

template <typename T>
static void Mem::delete2D(T** arr, const int NUM_ROWS) {
	if (arr == NULL) {
		return;
	}

	for (int i = 0; i < NUM_ROWS; i++) {
		delete[] arr[i];
	}

	delete[] arr;
}
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Rnd.h"

using namespace std;

class Arr {
private:
	template <typename T> static void quickSort(T*& arr, const int LEN, const int left, const int right, const bool DESC);

public:
	template <typename T> static string toString(T* ARR, const int LEN);
	template <typename T> static string toString(T** ARR, const int NUM_ROWS, const int NUM_COLS);
	template <typename T> static void copy(T*& dest, const T* SRC, const int LEN);
	template <typename T> static void copy(T**& dest, T** SRC, const int NUM_ROWS, const int NUM_COLS);
	template <typename T> static void set(T*& arr, const int LEN, const T VAL);
	template <typename T> static void set(T**& arr, const int NUM_ROWS, const int NUM_COLS, const T VAL);
	template <typename T> static T min(T* ARR, const int LEN);
	template <typename T> static T max(T* ARR, const int LEN);
	template <typename T> static int minIndex(T* ARR, const int LEN);
	template <typename T> static int maxIndex(T* ARR, const int LEN);
	template <typename T> static T minCol(T** ARR, const int NUM_ROWS, const int INDEX_COL);
	template <typename T> static T maxCol(T** ARR, const int NUM_ROWS, const int INDEX_COL);
	template <typename T> static void sort(T*& arr, const int LEN, const bool DESC);
	template <typename T> static void sort(vector<T>* vec);
	template <typename T> static void initRandomly(T*& arr, const int LEN, const float MIN, const float MAX);
	template <typename T> static void initRandomly(T**& arr, const int NUM_ROWS, const int NUM_COLS, const float MIN, const float MAX);
	template <typename T> static T sum(T* ARR, const int LEN);
	template <typename T> static float average(T* ARR, const int LEN);
};

template <typename T>
void Arr::quickSort(T*& arr, const int LEN, const int left, const int right, const bool DESC) {
	int i = left;
	int j = right;
	int temp;
	int pivot = arr[(left + right) / 2];

	if (DESC) {
		while (i <= j) {
			while (arr[i] > pivot) {
				i++;
			}

			while (arr[j] < pivot) {
				j--;
			}

			if (i <= j) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
				i++;
				j--;
			}
		}
	}
	else {
		while (i <= j) {
			while (arr[i] < pivot) {
				i++;
			}

			while (arr[j] > pivot) {
				j--;
			}

			if (i <= j) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
				i++;
				j--;
			}
		}
	}

	if (left < j) {
		quickSort(arr, LEN, left, j, DESC);
	}

	if (i < right) {
		quickSort(arr, LEN, i, right, DESC);
	}
}

template <typename T>
string Arr::toString(T* ARR, const int LEN) {
	string result = "[ ";

	for (int i = 0; i < LEN; i++) {
		result += to_string(ARR[i]) + " ";
	}
	result += "]";

	return result;
}

template <typename T>
string Arr::toString(T** ARR, const int NUM_ROWS, const int NUM_COLS) {
	string result = "[";

	for (int i = 0; i < NUM_ROWS; i++) {
		if (i < NUM_ROWS - 1) {
			result += toString(ARR[i], NUM_COLS) + "\n";
		}
		else {
			result += toString(ARR[i], NUM_COLS);
		}
	}
	result += "]";

	return result;
}

template <typename T>
void Arr::copy(T*& dest, const T* SRC, const int LEN) {
	memcpy(dest, SRC, sizeof(T) * LEN);
}

template <typename T>
void Arr::copy(T**& dest, T** SRC, const int NUM_ROWS, const int NUM_COLS) {
	for (int i = 0; i < NUM_ROWS; i++) {
		memcpy(dest[i], SRC[i], sizeof(T) * NUM_COLS);
	}
}

template <typename T>
void Arr::set(T*& arr, const int LEN, const T VAL) {
	for (int i = 0; i < LEN; i++) {
		arr[i] = VAL;
	}
}

template <typename T>
void Arr::set(T**& arr, const int NUM_ROWS, const int NUM_COLS, const T VAL) {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			arr[i][j] = VAL;
		}
	}
}

template <typename T>
T Arr::min(T* ARR, const int LEN) {
	T minVal = ARR[0];

	for (int i = 1; i < LEN; i++) {
		if (ARR[i] < minVal) {
			minVal = ARR[i];
		}
	}

	return minVal;
}

template <typename T>
T Arr::max(T* ARR, const int LEN) {
	T maxVal = ARR[0];

	for (int i = 1; i < LEN; i++) {
		if (ARR[i] > maxVal) {
			maxVal = ARR[i];
		}
	}

	return maxVal;
}

template <typename T>
int Arr::minIndex(T* ARR, const int LEN) {
	int minIndex = 0;
	T minVal = ARR[0];

	for (int i = 1; i < LEN; i++) {
		if (ARR[i] < minVal) {
			minIndex = i;
			minVal = ARR[i];
		}
	}

	return minIndex;
}

template <typename T>
int Arr::maxIndex(T* ARR, const int LEN) {
	int maxIndex = 0;
	T maxVal = ARR[0];

	for (int i = 1; i < LEN; i++) {
		if (ARR[i] > maxVal) {
			maxIndex = i;
			maxVal = ARR[i];
		}
	}

	return maxIndex;
}

template <typename T>
T Arr::minCol(T** ARR, const int NUM_ROWS, const int INDEX_COL) {
	T minVal = ARR[0][INDEX_COL];

	for (int i = 1; i < NUM_ROWS; i++) {
		if (ARR[i][INDEX_COL] < minVal) {
			minVal = ARR[i][INDEX_COL];
		}
	}

	return minVal;
}

template <typename T>
T Arr::maxCol(T** ARR, const int NUM_ROWS, const int INDEX_COL) {
	T maxVal = ARR[0][INDEX_COL];

	for (int i = 1; i < NUM_ROWS; i++) {
		if (ARR[i][INDEX_COL] > maxVal) {
			maxVal = ARR[i][INDEX_COL];
		}
	}

	return maxVal;
}

template <typename T>
void Arr::sort(T*& arr, const int LEN, const bool DESC) {
	quickSort<T>(arr, LEN, 0, LEN - 1, DESC);
}

template <typename T>
void Arr::sort(vector<T>* vec) {
	std::sort(vec->begin(), vec->end());
}

template <typename T>
void Arr::initRandomly(T*& arr, const int LEN, const float MIN, const float MAX) {
	for (int i = 0; i < LEN; i++) {
		arr[i] = (T)Rnd::rand<T>(MIN, MAX);
	}
}

template <typename T>
void Arr::initRandomly(T**& arr, const int NUM_ROWS, const int NUM_COLS, const float MIN, const float MAX) {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			arr[i][j] = (T)Rnd::rand<T>(MIN, MAX);
		}
	}
}

template <typename T>
T Arr::sum(T* ARR, const int LEN) {
	T sum = 0;

	for (int i = 0; i < LEN; i++) {
		sum += ARR[i];
	}

	return sum;
}

template <typename T>
float Arr::average(T* ARR, const int LEN) {
	return sum<T>(ARR, LEN) / (float)LEN;
}
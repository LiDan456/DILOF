#pragma once

#include <iostream>

#define PI			3.14159265359
#define EXP			2.71828182845
#define EPSILON		0.000001

class Math {
public:
	template <typename T> static void vectorAdd(T*& v1, const T* v2, const int DIM);
	template <typename T> static void vectorAdd(T*& v, const T VAL, const int DIM);
	template <typename T> static void vectorSub(T*& v1, const T* v2, const int DIM);
	template <typename T> static void vectorSub(T*& v, const T VAL, const int DIM);
	template <typename T> static void vectorMul(T*& v1, const T* v2, const int DIM);
	template <typename T> static void vectorMul(T*& v, const T VAL, const int DIM);
	template <typename T> static void vectorDiv(T*& v1, const T* v2, const int DIM);
	template <typename T> static void vectorDiv(T*& v, const T VAL, const int DIM);
	template <typename T> static float eucDist(const T* v1, const T* v2, const int DIM);
	template <typename T> static float gravity(const T* v1, const int MASS_V1, const T* v2, const int MASS_V2, const int DIM);
	template <typename T> static float EMD(const T** optFlow, const T** distances, const int SIZE_DIST1, const int SIZE_DIST2);
	template <typename T> static float average(const T* data, const int NUM);
	template <typename T> static float std(const T* data, const int NUM);
	template <typename T> static T min(const T val1, const T val2);
	template <typename T> static T max(const T val1, const T val2);
	template <typename T> static float trapz(const T* pointsX, const T* pointsY, const int DIM);
	template <typename T> static T gamma(const T VAL);
	static float sigmoid(const float VAL);
	static float tanH(const float VAL);
};

template <typename T>
void Math::vectorAdd(T*& v1, const T* v2, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v1[i] = v1[i] + v2[i];
	}
}

template <typename T>
void Math::vectorAdd(T*& v, const T VAL, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v[i] = v[i] + VAL;
	}
}

template <typename T>
void Math::vectorSub(T*& v1, const T* v2, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v1[i] = v1[i] - v2[i];
	}
}

template <typename T>
void Math::vectorSub(T*& v, const T VAL, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v[i] = v[i] - VAL;
	}
}

template <typename T>
void Math::vectorMul(T*& v1, const T* v2, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v1[i] = v1[i] * v2[i];
	}
}

template <typename T>
void Math::vectorMul(T*& v, const T VAL, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v[i] = v[i] * VAL;
	}
}

template <typename T>
void Math::vectorDiv(T*& v1, const T* v2, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v1[i] = v1[i] / v2[i];
	}
}

template <typename T>
void Math::vectorDiv(T*& v, const T VAL, const int DIM) {
	for (int i = 0; i < DIM; i++) {
		v[i] = v[i] / VAL;
	}
}

template <typename T>
float Math::eucDist(const T* v1, const T* v2, const int DIM) {
	float distance = 0;

	for (int i = 0; i < DIM; i++) {
		distance += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}

	return (float)std::sqrt(distance);
}

template <typename T>
float Math::gravity(const T* v1, const int MASS_V1, const T* v2, const int MASS_V2, const int DIM) {
	float distance = 0;

	for (int i = 0; i < DIM; i++) {
		distance += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}

	return (MASS_V1 * MASS_V2) / distance;
}

template <typename T>
float Math::EMD(const T** optFlow, const T** distances, const int SIZE_DIST1, const int SIZE_DIST2) {
	float sumFlow = 0;
	float sumWork = 0;

	for (int i = 0; i < SIZE_DIST1; i++) {
		for (int j = 0; j < SIZE_DIST2; j++) {
			sumFlow += optFlow[i][j];
			sumWork += optFlow[i][j] * distances[i][j];
		}
	}

	return sumWork / sumFlow;
}

template <typename T>
float Math::average(const T* data, const int NUM) {
	float sum = 0;

	for (int i = 0; i < NUM; i++) {
		sum += data[i];
	}

	return sum / (float)NUM;
}

template <typename T>
float Math::std(const T* data, const int NUM) {
	const float average = Math::average<T>(data, NUM);
	float sum = 0;

	for (int i = 0; i < NUM; i++) {
		sum += (data[i] - average) * (data[i] - average);
	}

	return sqrt(sum / (float)NUM);
}

template <typename T>
T Math::min(const T val1, const T val2) {
	if (val1 < val2) {
		return val1;
	}
	else {
		return val2;
	}
}

template <typename T>
T Math::max(const T val1, const T val2) {
	if (val1 > val2) {
		return val1;
	}
	else {
		return val2;
	}
}

template <typename T>
float Math::trapz(const T* pointsX, const T* pointsY, const int DIM) {
	float sum = 0;

	for (int i = 0; i < DIM - 1; i++) {
		sum += abs((pointsX[i + 1] - pointsX[i])) * min<T>(pointsY[i], pointsY[i + 1]);
		sum += (abs(pointsX[i + 1] - pointsX[i]) * abs(pointsY[i] - pointsY[i + 1])) / 2.0f;
	}

	return sum;
}

template <typename T>
T Math::gamma(const T VAL) {
	T result = 0;

	if (floor(VAL) == VAL) {
		result = 1;

		for (int i = 2; i <= VAL; i++) {
			result *= i;
		}
	}
	else {
		result = (T)(sqrt(2 * PI * VAL) * pow(VAL / EXP, VAL));
	}

	return result;
}

float Math::sigmoid(const float VAL) {
	return 1 / (float)(1 + exp(-VAL));
}

float Math::tanH(const float VAL) {
	return (2 / (float)(1 + exp(-2 * VAL))) - 1;
}
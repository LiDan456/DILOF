#pragma once

#include <vector>
#include <string>

using namespace std;

class Str {
public:
	static int split(string*& dest, const string str, const string delimeter);
	static int splitAsInt(int*& dest, const string str, const string delimeter);
	static int splitAsFloat(float*& dest, const string str, const string delimeter);
	static int toInt(const string str);
	static float toFloat(const string str);
};

int Str::split(string*& dest, const string str, const string delimeter) {
	const int LENGTH = (const int)str.length();
	const int LENGTH_DELIM = (const int)delimeter.length();

	int bound = 0;
	vector<string> result;

	for (int i = 0; i < LENGTH; i++) {
		if (str.compare(i, LENGTH_DELIM, delimeter) == 0 && i > 0) {
			result.push_back(str.substr(bound, i - bound));
			bound = i + LENGTH_DELIM;
		}
	}

	if (bound < LENGTH) {
		result.push_back(str.substr(bound, LENGTH - bound));
	}

	const int NUM_WORDS = (const int)result.size();
	dest = new string[NUM_WORDS];

	for (int i = 0; i < NUM_WORDS; i++) {
		dest[i] = result[i];
	}

	return NUM_WORDS;
}

int Str::splitAsInt(int*& dest, const string str, const string delimeter) {
	const int LENGTH = (const int)str.length();
	const int LENGTH_DELIM = (const int)delimeter.length();

	int bound = 0;
	vector<int> result;

	for (int i = 0; i < LENGTH; i++) {
		if (str.compare(i, LENGTH_DELIM, delimeter) == 0 && i > 0) {
			result.push_back(Str::toInt(str.substr(bound, i - bound)));
			bound = i + LENGTH_DELIM;
		}
	}

	if (bound < LENGTH) {
		result.push_back(Str::toInt(str.substr(bound, LENGTH - bound)));
	}

	const int NUM_DATA = (const int)result.size();
	dest = new int[NUM_DATA];

	for (int i = 0; i < NUM_DATA; i++) {
		dest[i] = result[i];
	}

	return NUM_DATA;
}

int Str::splitAsFloat(float*& dest, const string str, const string delimeter) {
	const int LENGTH = (const int)str.length();
	const int LENGTH_DELIM = (const int)delimeter.length();

	int bound = 0;
	vector<float> result;

	for (int i = 0; i < LENGTH; i++) {
		if (str.compare(i, LENGTH_DELIM, delimeter) == 0 && i > 0) {
			result.push_back(Str::toFloat(str.substr(bound, i - bound)));
			bound = i + LENGTH_DELIM;
		}
	}

	if (bound < LENGTH) {
		result.push_back(Str::toFloat(str.substr(bound, LENGTH - bound)));
	}

	const int NUM_DATA = (const int)result.size();
	dest = new float[NUM_DATA];

	for (int i = 0; i < NUM_DATA; i++) {
		dest[i] = result[i];
	}

	return NUM_DATA;
}

int Str::toInt(const string str) {
	return atoi(str.c_str());
}

float Str::toFloat(const string str) {
	return (float)atof(str.c_str());
}
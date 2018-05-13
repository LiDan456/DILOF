#pragma once

#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

chrono::time_point<chrono::system_clock> startTime;
chrono::time_point<chrono::system_clock> endTime;
chrono::duration<double> elapsedTime;

class Time {
public:
	static const int TIME_SECOND = 0;
	static const int TIME_MILLISECOND = 1;

	static void startTimeChecking();
	static float stopTimeChecking(const int TIME_UNIT);
};

void Time::startTimeChecking() {
	startTime = chrono::system_clock::now();
}

float Time::stopTimeChecking(const int TIME_UNIT) {
	endTime = chrono::system_clock::now();
	elapsedTime = endTime - startTime;

	if (TIME_UNIT == Time::TIME_SECOND) {
		return (float)elapsedTime.count();
	}
	else if (TIME_UNIT == Time::TIME_MILLISECOND) {
		return (float)elapsedTime.count() * 1000;
	}

	return -1;
}
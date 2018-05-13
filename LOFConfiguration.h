#pragma once

class LOFConfiguration {
public:
	int TYPE_DIST = 0;
	int DIM_DATA_POINT = 0;
	int K = 0;
	float THRESHOLD = 0;
	int MAX_NUM_INSTANCES = 0;

	LOFConfiguration(const int DIM_DATA_POINT, const int K, const float THRESHOLD, const int MAX_NUM_INSTANCES);
};

LOFConfiguration::LOFConfiguration(const int DIM_DATA_POINT, const int K, const float THRESHOLD, const int MAX_NUM_INSTANCES) {
	this->TYPE_DIST = TYPE_DIST;
	this->DIM_DATA_POINT = DIM_DATA_POINT;
	this->K = K;
	this->THRESHOLD = THRESHOLD;
	this->MAX_NUM_INSTANCES = MAX_NUM_INSTANCES;
}
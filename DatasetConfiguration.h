#pragma once

#include <string>

#include "LibNGS\Dataset.h"

using namespace std;

class DatasetConfiguration {
public:
	static const int UNLABELLED = Dataset::UNLABELLED;
	static const int LABELLED = Dataset::LABELLED;
	static const int TYPE_NOISE_NONE = 0;
	static const int TYPE_NOISE_UNIFORM = 1;
	static const int TYPE_NOISE_GAUSSIAN = 2;
	static const int TYPE_NOISE_CLASS = 3;

	int DATASET_TYPE = -1;
	string DATA_FILE_NAME;
	string DELIMITER;
	bool SHUFFLE = false;
	bool NORMALIZE = false;
	int TYPE_NOISE = DatasetConfiguration::TYPE_NOISE_NONE;
	float NOISE_RATIO = 0;
	float NOISE_MIN = 0;
	float NOISE_MAX = 0;
	float NOISE_MEAN = 0;
	float NOISE_STD = 0;
	int NOISE_CLASS_INDEX = 0;

	DatasetConfiguration(const int DATASET_TYPE, const string DATA_FILE_NAME, const string DELIMITER, const bool SHUFFLE, const bool NORMALIZE);
	void addUniformNoise(const float RATIO_PERCENT, const float MIN, const float MAX);
	void addUniformNoise(const float RATIO_PERCENT);
	void addGaussianNoise(const float RATIO_PERCENT, const float MEAN, const float STD);
	void setNoiseClass(const int INDEX_CLASS);
};

DatasetConfiguration::DatasetConfiguration(const int DATASET_TYPE, const string DATA_FILE_NAME, const string DELIMITER, const bool SHUFFLE, const bool NORMALIZE) {
	this->DATASET_TYPE = DATASET_TYPE;
	this->DATA_FILE_NAME = DATA_FILE_NAME;
	this->DELIMITER = DELIMITER;
	this->SHUFFLE = SHUFFLE;
	this->NORMALIZE = NORMALIZE;
}

void DatasetConfiguration::addUniformNoise(const float RATIO_PERCENT, const float MIN, const float MAX) {
	TYPE_NOISE = DatasetConfiguration::TYPE_NOISE_UNIFORM;
	NOISE_RATIO = RATIO_PERCENT;
	NOISE_MIN = MIN;
	NOISE_MAX = MAX;
}

void DatasetConfiguration::addUniformNoise(const float RATIO_PERCENT) {
	TYPE_NOISE = DatasetConfiguration::TYPE_NOISE_UNIFORM;
	NOISE_RATIO = RATIO_PERCENT;
	NOISE_MIN = 0;
	NOISE_MAX = 0;
}

void DatasetConfiguration::addGaussianNoise(const float RATIO_PERCENT, const float MEAN, const float STD) {
	TYPE_NOISE = DatasetConfiguration::TYPE_NOISE_GAUSSIAN;
	NOISE_RATIO = RATIO_PERCENT;
	NOISE_MEAN = MEAN;
	NOISE_STD = STD;
}

void DatasetConfiguration::setNoiseClass(const int INDEX_CLASS) {
	TYPE_NOISE = DatasetConfiguration::TYPE_NOISE_CLASS;
	NOISE_CLASS_INDEX = INDEX_CLASS;
}
#pragma once

#include <iostream>

#include "Math.h"
#include "Tuple.h"

class Instance {
protected:
	int DIM_DATA_VECTOR = 0;
	int DIM_TARGET_VECTOR = 0;

	void setLabel();

public:
	int id = -1;
	int label = -1;
	float* dataVector = NULL;
	float* targetVector = NULL;

	Instance(const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int ID = -1);
	Instance(float* dataVector, const int DIM_DATA_VECTOR, float* targetVector, const int DIM_TARGET_VECTOR, const int ID = -1);
	Instance(Instance* instance);
	~Instance();
	int getDimDataVector();
	int getDimTargetVector();
	void copy(Instance* instance);
	void copy(const float* dataVector, const float* targetVector);
	float eucDistDV(Instance* instance);
	float eucDistTV(Instance* instance);
	Tuple<int, float>* nearestInstance(Instance** INSTANCES, const int NUM);
	Tuple<int, float>* furthermostInstance(Instance** INSTANCES, const int NUM);
};

void Instance::setLabel() {
	for (int i = 0; i < DIM_TARGET_VECTOR; i++) {
		if (targetVector[i] == 1) {
			label = i;
			return;
		}
	}
}

Instance::Instance(const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int ID) {
	this->DIM_DATA_VECTOR = DIM_DATA_VECTOR;
	this->DIM_TARGET_VECTOR = DIM_TARGET_VECTOR;
	this->id = ID;
	this->dataVector = new float[this->DIM_DATA_VECTOR];

	if (this->DIM_TARGET_VECTOR > 0) {
		this->targetVector = new float[this->DIM_TARGET_VECTOR];
	}
}

Instance::Instance(float* dataVector, const int DIM_DATA_VECTOR, float* targetVector, const int DIM_TARGET_VECTOR, const int ID) {
	this->DIM_DATA_VECTOR = DIM_DATA_VECTOR;
	this->DIM_TARGET_VECTOR = DIM_TARGET_VECTOR;
	this->id = ID;
	this->dataVector = new float[this->DIM_DATA_VECTOR];
	Arr::copy<float>(this->dataVector, dataVector, this->DIM_DATA_VECTOR);

	if (targetVector != NULL) {
		this->targetVector = new float[this->DIM_TARGET_VECTOR];
		Arr::copy<float>(this->targetVector, targetVector, this->DIM_TARGET_VECTOR);
		setLabel();
	}
}

Instance::Instance(Instance* instance) {
	this->DIM_DATA_VECTOR = instance->getDimDataVector();
	this->DIM_TARGET_VECTOR = instance->getDimTargetVector();
	this->id = instance->id;
	this->label = instance->label;
	this->dataVector = new float[this->DIM_DATA_VECTOR];

	Arr::copy<float>(this->dataVector, instance->dataVector, this->DIM_DATA_VECTOR);

	if (DIM_TARGET_VECTOR > 0) {
		this->targetVector = new float[this->DIM_TARGET_VECTOR];
		Arr::copy<float>(this->targetVector, instance->targetVector, this->DIM_TARGET_VECTOR);
	}
}

Instance::~Instance() {
	delete[] dataVector;
	delete[] targetVector;
}

int Instance::getDimDataVector() {
	return DIM_DATA_VECTOR;
}

int Instance::getDimTargetVector() {
	return DIM_TARGET_VECTOR;
}

void Instance::copy(Instance* instance) {
	Arr::copy<float>(dataVector, instance->dataVector, DIM_DATA_VECTOR);
	Arr::copy<float>(targetVector, instance->targetVector, DIM_TARGET_VECTOR);
	this->id = instance->id;
	setLabel();
}

void Instance::copy(const float* dataVector, const float* targetVector) {
	Arr::copy<float>(this->dataVector, dataVector, DIM_DATA_VECTOR);
	Arr::copy<float>(this->targetVector, targetVector, DIM_TARGET_VECTOR);
	setLabel();
}

float Instance::eucDistDV(Instance* instance) {
	return Math::eucDist<float>(this->dataVector, instance->dataVector, DIM_DATA_VECTOR);
}

float Instance::eucDistTV(Instance* instance) {
	return Math::eucDist<float>(this->targetVector, instance->targetVector, DIM_TARGET_VECTOR);
}

Tuple<int, float>* Instance::nearestInstance(Instance** INSTANCES, const int NUM) {
	Tuple<int, float>* result = new Tuple<int, float>(-1, -1);
	float minDist = this->eucDistDV(INSTANCES[0]);
	int minDistIndex = 0;
	float dist = 0;

	for (int i = 1; i < NUM; i++) {
		dist = this->eucDistDV(INSTANCES[i]);

		if (dist < minDist) {
			minDist = dist;
			minDistIndex = i;
		}
	}

	result->first = minDistIndex;
	result->second = minDist;

	return result;
}

Tuple<int, float>* Instance::furthermostInstance(Instance** INSTANCES, const int NUM) {
	Tuple<int, float>* result = new Tuple<int, float>(-1, -1);
	float maxDist = this->eucDistDV(INSTANCES[0]);
	int maxDistIndex = 0;
	float dist = 0;

	for (int i = 1; i < NUM; i++) {
		dist = this->eucDistDV(INSTANCES[i]);

		if (dist > maxDist) {
			maxDist = dist;
			maxDistIndex = i;
		}
	}

	result->first = maxDistIndex;
	result->second = maxDist;

	return result;
}
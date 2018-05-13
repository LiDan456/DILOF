#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Arr.h"
#include "Mem.h"
#include "Rnd.h"
#include "Str.h"
#include "Instance.h"

using namespace std;

class Dataset {
protected:
	int TYPE = -1;
	int DIM_DATA_VECTOR = 0;
	int DIM_TARGET_VECTOR = 0;
	int NUM_CLASSES = 0;
	int NUM_NOISED_INSTANCES = 0;

	void readDataFile(const string DATA_FILE_NAME, const string DELIMETER);
	void readTestFile(const string TEST_FILE_NAME, const string DELIMETER);
	string vectorToString(float* vector, const int DIM);

public:
	static const int LABELLED = 0;
	static const int UNLABELLED = 1;
	static const int TRAINING_SET = 0;
	static const int TEST_SET = 1;

	vector<Instance*> instances;
	vector<Instance*> testInstances;
	vector<int> noisedInstancesId;

	Dataset() { };
	Dataset(const int TYPE, const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int NUM_CLASSES);
	Dataset(const int TYPE, const string DATA_FILE_NAME, const string DELIMETER);
	Dataset(const int TYPE, const string DATA_FILE_NAME, const string TEST_DATA_FILE_NAME, const string DELIMETER);
	Dataset(Dataset* dataset);
	~Dataset();
	void initialize(const int TYPE, const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int NUM_CLASSES);
	void initialize(const int TYPE, const string DATA_FILE_NAME, const string DELIMETER);
	void clear();
	int getType();
	int getNumInstances();
	int getNumTestInstances();
	int getNumNoisedInstances();
	int getDimDataVector();
	int getDimTargetVector();
	int getNumClasses();
	void getDataVectors(float**& dataVectors);
	void getTargetVectors(float**& targetVectors);
	int getInstancesIdByClass(int*& instancesId, const int INDEX_CLASS);
	void addInstance(Instance* newInstance);
	void addInstance(Instance* newInstance, const int POS);
	void addInstanceCopy(Instance* newInstance);
	void addInstanceCopy(Instance* newInstance, const int POS);
	void addTestInstance(Instance* newInstance);
	void addTestInstance(Instance* newInstance, const int POS);
	void addTestInstanceCopy(Instance* newInstance);
	void addTestInstanceCopy(Instance* newInstance, const int POS);
	void removeInstance(const int POS);
	void removeInstance(const int POS_START, const int POS_END);
	void removeInstances(int* ARR, const int LEN);
	void removeTestInstance(const int POS);
	void removeTestInstance(const int POS_START, const int POS_END);
	void removeTestInstances(int* ARR, const int LEN);
	void normalize();
	void shuffle();
	void copy(Dataset* dataset);
	void addUniformNoise(const float RATIO_PERCENT, const float MIN, const float MAX);
	void addUniformNoise(const float RATIO_PERCENT);
	void addGaussianNoise(const float RATIO_PERCENT, const float MEAN, const float STD);
	int idToIndex(const int ID);
	int indexToId(const int INDEX);
	void splitTestDataset(const float RATIO);
	void print();
	void save(const string FILE_NAME);
};

void Dataset::readDataFile(const string DATA_FILE_NAME, const string DELIMETER) {
	ifstream file(DATA_FILE_NAME);
	string temp;
	int numInstances= 0;
	float* tempData = NULL;
	float* dataVector = NULL;
	float* targetVector = NULL;

	getline(file, temp);
	numInstances = Str::toInt(temp);

	getline(file, temp);
	DIM_DATA_VECTOR = Str::toInt(temp);

	if (TYPE == LABELLED) {
		getline(file, temp);
		DIM_TARGET_VECTOR = Str::toInt(temp);

		getline(file, temp);
		NUM_CLASSES = Str::toInt(temp);
	}
	
	for (int i = 0; i < numInstances; i++) {
		dataVector = new float[DIM_DATA_VECTOR];
		targetVector = new float[DIM_TARGET_VECTOR];

		getline(file, temp);
		Str::splitAsFloat(tempData, temp, DELIMETER);
		Arr::copy<float>(dataVector, tempData, DIM_DATA_VECTOR);

		if (TYPE == LABELLED) {
			Arr::copy<float>(targetVector, &tempData[DIM_DATA_VECTOR], DIM_TARGET_VECTOR);
		}

		instances.push_back(new Instance(dataVector, DIM_DATA_VECTOR, targetVector, DIM_TARGET_VECTOR, i));

		delete[] tempData;
		delete[] dataVector;
		delete[] targetVector;
	}

	file.close();
}

void Dataset::readTestFile(const string TEST_FILE_NAME, const string DELIMETER) {
	ifstream file(TEST_FILE_NAME);
	string temp;
	int numTestInstances = 0;
	float* tempData = NULL;

	getline(file, temp);
	numTestInstances = Str::toInt(temp);

	for (int i = 0; i < numTestInstances; i++) {
		Instance* instance = new Instance(DIM_DATA_VECTOR, DIM_TARGET_VECTOR, i);

		getline(file, temp);
		Str::splitAsFloat(tempData, temp, DELIMETER);
		Arr::copy<float>(instance->dataVector, tempData, DIM_DATA_VECTOR);

		if (TYPE == LABELLED) {
			Arr::copy<float>(instance->targetVector, &tempData[DIM_DATA_VECTOR], DIM_TARGET_VECTOR);
		}

		testInstances.push_back(instance);

		delete[] tempData;
	}

	file.close();
}

string Dataset::vectorToString(float* vector, const int DIM) {
	string strVector = "";

	for (int i = 0; i < DIM; i++) {
		if (i < DIM - 1) {
			strVector += to_string(vector[i]) + ",";
		}
		else {
			strVector += to_string(vector[i]);
		}
	}

	return strVector;
}

Dataset::Dataset(const int TYPE, const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int NUM_CLASSES) {
	initialize(TYPE, DIM_DATA_VECTOR, DIM_TARGET_VECTOR, NUM_CLASSES);
}

Dataset::Dataset(const int TYPE, const string DATA_FILE_NAME, const string DELIMETER) {
	this->TYPE = TYPE;

	readDataFile(DATA_FILE_NAME, DELIMETER);
}

Dataset::Dataset(const int TYPE, const string DATA_FILE_NAME, const string TEST_DATA_FILE_NAME, const string DELIMETER) {
	this->TYPE = TYPE;

	readDataFile(DATA_FILE_NAME, DELIMETER);
	readTestFile(TEST_DATA_FILE_NAME, DELIMETER);
}

Dataset::Dataset(Dataset* dataset) {
	copy(dataset);
}

Dataset::~Dataset() {
	clear();
}

void Dataset::initialize(const int TYPE, const int DIM_DATA_VECTOR, const int DIM_TARGET_VECTOR, const int NUM_CLASSES) {
	this->TYPE = TYPE;
	this->DIM_DATA_VECTOR = DIM_DATA_VECTOR;
	this->DIM_TARGET_VECTOR = DIM_TARGET_VECTOR;
	this->NUM_CLASSES = NUM_CLASSES;
}

void Dataset::initialize(const int TYPE, const string DATA_FILE_NAME, const string DELIMETER) {
	this->TYPE = TYPE;

	readDataFile(DATA_FILE_NAME, DELIMETER);
}

void Dataset::clear() {
	const int NUM_INSTANCES = (const int)instances.size();
	const int NUM_TEST_INSTANCES = (const int)testInstances.size();

	// Clear the stored data.
	for (int i = 0; i < NUM_INSTANCES; i++) {
		delete instances[i];
	}
	instances.clear();

	for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
		delete testInstances[i];
	}
	testInstances.clear();

	// Clear the factors.
	TYPE = -1;
	DIM_DATA_VECTOR = 0;
	DIM_TARGET_VECTOR = 0;
	NUM_CLASSES = 0;
	NUM_NOISED_INSTANCES = 0;
}

int Dataset::getType() {
	return TYPE;
}

int Dataset::getNumInstances() {
	return (int)instances.size();
}

int Dataset::getNumTestInstances() {
	return (int)testInstances.size();
}

int Dataset::getNumNoisedInstances() {
	return NUM_NOISED_INSTANCES;
}

int Dataset::getDimDataVector() {
	return DIM_DATA_VECTOR;
}

int Dataset::getDimTargetVector() {
	return DIM_TARGET_VECTOR;
}

int Dataset::getNumClasses() {
	return NUM_CLASSES;
}

void Dataset::getDataVectors(float**& dataVectors) {
	const int NUM_INSTANCES = (const int)getNumInstances();

	dataVectors = new float*[NUM_INSTANCES];

	for (int i = 0; i < NUM_INSTANCES; i++) {
		dataVectors[i] = instances[i]->dataVector;
	}
}

void Dataset::getTargetVectors(float**& targetVectors) {
	const int NUM_INSTANCES = (const int)getNumInstances();

	targetVectors = new float*[NUM_INSTANCES];

	for (int i = 0; i < NUM_INSTANCES; i++) {
		targetVectors[i] = instances[i]->targetVector;
	}
}

int Dataset::getInstancesIdByClass(int*& instancesId, const int INDEX_CLASS) {
	const int NUM_INSTANCES = (const int)instances.size();
	vector<int> temp;

	for (int i = 0; i < NUM_INSTANCES; i++) {
		if (instances[i]->label == INDEX_CLASS) {
			temp.push_back(instances[i]->id);
		}
	}

	const int NUM_INSTANCES_BY_CLASS = (const int)temp.size();

	instancesId = new int[NUM_INSTANCES_BY_CLASS];
	for (int i = 0; i < NUM_INSTANCES_BY_CLASS; i++) {
		instancesId[i] = temp[i];
	}

	return NUM_INSTANCES_BY_CLASS;
}

void Dataset::addInstance(Instance* newInstance) {
	instances.push_back(newInstance);
}

void Dataset::addInstance(Instance* newInstance, const int POS) {
	instances.insert(instances.begin() + POS, newInstance);
}

void Dataset::addInstanceCopy(Instance* newInstance) {
	instances.push_back(new Instance(newInstance));
}

void Dataset::addInstanceCopy(Instance* newInstance, const int POS) {
	instances.insert(instances.begin() + POS, new Instance(newInstance));
}

void Dataset::addTestInstance(Instance* newInstance) {
	testInstances.push_back(new Instance(newInstance));
}

void Dataset::addTestInstance(Instance* newInstance, const int POS) {
	testInstances.insert(testInstances.begin() + POS, new Instance(newInstance));
}

void Dataset::addTestInstanceCopy(Instance* newInstance) {
	testInstances.push_back(new Instance(newInstance));
}

void Dataset::addTestInstanceCopy(Instance* newInstance, const int POS) {
	testInstances.insert(testInstances.begin() + POS, new Instance(newInstance));
}

void Dataset::removeInstance(const int POS) {
	delete instances[POS];
	instances.erase(instances.begin() + POS);
}

void Dataset::removeInstance(const int POS_START, const int POS_END) {
	for (int i = POS_START; i < POS_END; i++) {
		delete instances[i];
	}

	instances.erase(instances.begin() + POS_START, instances.begin() + POS_END);
}

void Dataset::removeInstances(int* ARR, const int LEN) {
	Arr::sort<int>(ARR, LEN, true);

	for (int i = 0; i < LEN; i++) {
		removeInstance(ARR[i]);
	}
}

void Dataset::removeTestInstance(const int POS) {
	testInstances.erase(testInstances.begin() + POS);
}

void Dataset::removeTestInstance(const int POS_START, const int POS_END) {
	testInstances.erase(testInstances.begin() + POS_START, testInstances.begin() + POS_END);
}

void Dataset::removeTestInstances(int* ARR, const int LEN) {
	Arr::sort<int>(ARR, LEN, true);

	for (int i = 0; i < LEN; i++) {
		removeTestInstance(ARR[i]);
	}
}

void Dataset::normalize() {
	const int NUM_INSTANCES = (const int)instances.size();
	const int NUM_TEST_INSTANCES = (const int)testInstances.size();
	float minVal;
	float maxVal;

	for (int j = 0; j < DIM_DATA_VECTOR; j++) {
		minVal = instances[0]->dataVector[j];
		maxVal = instances[0]->dataVector[j];

		for (int i = 1; i < NUM_INSTANCES; i++) {
			if (instances[i]->dataVector[j] < minVal) {
				minVal = instances[i]->dataVector[j];
			}

			if (instances[i]->dataVector[j] > maxVal) {
				maxVal = instances[i]->dataVector[j];
			}
		}

		for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
			if (testInstances[i]->dataVector[j] < minVal) {
				minVal = testInstances[i]->dataVector[j];
			}

			if (testInstances[i]->dataVector[j] > maxVal) {
				maxVal = testInstances[i]->dataVector[j];
			}
		}

		for (int i = 0; i < NUM_INSTANCES; i++) {
			instances[i]->dataVector[j] = (instances[i]->dataVector[j] - minVal) / (maxVal - minVal);
		}

		for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
			testInstances[i]->dataVector[j] = (testInstances[i]->dataVector[j] - minVal) / (maxVal - minVal);
		}
	}
}

void Dataset::shuffle() {
	const int NUM_INSTANCES = (const int)instances.size();
	const int NUM_TEST_INSTANCES = (const int)testInstances.size();
	int* randSeq = NULL;
	float* temp = NULL;

	randSeq = new int[NUM_INSTANCES];
	Rnd::randSeq<int>(randSeq, 0, NUM_INSTANCES, NUM_INSTANCES, false);

	for (int i = 0; i < NUM_INSTANCES; i++) {
		temp = instances[i]->dataVector;
		instances[i]->dataVector = instances[randSeq[i]]->dataVector;
		instances[randSeq[i]]->dataVector = temp;

		if (DIM_TARGET_VECTOR > 0) {
			temp = instances[i]->targetVector;
			instances[i]->targetVector = instances[randSeq[i]]->targetVector;
			instances[randSeq[i]]->targetVector = temp;
		}
	}

	Rnd::randSeq<int>(randSeq, 0, NUM_TEST_INSTANCES, NUM_TEST_INSTANCES, false);

	for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
		temp = testInstances[i]->dataVector;
		testInstances[i]->dataVector = testInstances[randSeq[i]]->dataVector;
		testInstances[randSeq[i]]->dataVector = temp;

		if (DIM_TARGET_VECTOR > 0) {
			temp = testInstances[i]->targetVector;
			testInstances[i]->targetVector = testInstances[randSeq[i]]->targetVector;
			testInstances[randSeq[i]]->targetVector = temp;
		}
	}
}

void Dataset::copy(Dataset* dataset) {
	clear();

	const int NUM_INSTANCES = (const int)dataset->getNumInstances();
	const int NUM_TEST_INSTANCES = (const int)dataset->getNumTestInstances();

	TYPE = dataset->getType();
	DIM_DATA_VECTOR = dataset->getDimDataVector();
	DIM_TARGET_VECTOR = dataset->getDimTargetVector();
	NUM_CLASSES = dataset->getNumClasses();
	NUM_NOISED_INSTANCES = dataset->getNumNoisedInstances();

	for (int i = 0; i < NUM_INSTANCES; i++) {
		addInstance(new Instance(dataset->instances[i]));
	}

	for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
		addTestInstance(new Instance(dataset->testInstances[i]));
	}

	for (int i = 0; i < NUM_NOISED_INSTANCES; i++) {
		noisedInstancesId.push_back(dataset->noisedInstancesId[i]);
	}
}

void Dataset::addUniformNoise(const float RATIO_PERCENT, const float MIN, const float MAX) {
	noisedInstancesId.clear();

	const int NUM_INSTANCES = getNumInstances();

	NUM_NOISED_INSTANCES = (const int)(NUM_INSTANCES * RATIO_PERCENT);

	int* pos = new int[NUM_NOISED_INSTANCES];

	Rnd::randSeq<int>(pos, 0, NUM_INSTANCES, NUM_NOISED_INSTANCES, false);
	for (int i = 0; i < NUM_NOISED_INSTANCES; i++) {
		for (int j = 0; j < DIM_DATA_VECTOR; j++) {
			instances[pos[i]]->dataVector[j] += Rnd::rand<float>(MIN, MAX);
		}

		noisedInstancesId.push_back(instances[pos[i]]->id);
	}

	Arr::sort<int>(&noisedInstancesId);

	delete[] pos;
}

void Dataset::addUniformNoise(const float RATIO_PERCENT) {
	noisedInstancesId.clear();

	const int NUM_INSTANCES = getNumInstances();
	float** dataVectors = NULL;
	float* min = new float[DIM_DATA_VECTOR];
	float* max = new float[DIM_DATA_VECTOR];

	NUM_NOISED_INSTANCES = (const int)(NUM_INSTANCES * RATIO_PERCENT);

	int* pos = new int[NUM_NOISED_INSTANCES];

	Rnd::randSeq<int>(pos, 0, NUM_INSTANCES, NUM_NOISED_INSTANCES, false);
	getDataVectors(dataVectors);

	for (int i = 0; i < DIM_DATA_VECTOR; i++) {
		min[i] = Arr::minCol<float>(dataVectors, NUM_INSTANCES, i);
		max[i] = Arr::maxCol<float>(dataVectors, NUM_INSTANCES, i);
	}

	for (int i = 0; i < NUM_NOISED_INSTANCES; i++) {
		for (int j = 0; j < DIM_DATA_VECTOR; j++) {
			dataVectors[pos[i]][j] += Rnd::rand<float>(min[j], max[j]);
		}

		noisedInstancesId.push_back(instances[pos[i]]->id);
	}

	Arr::sort<int>(&noisedInstancesId);

	delete[] dataVectors;
	delete[] min;
	delete[] max;
	delete[] pos;
}

void Dataset::addGaussianNoise(const float RATIO_PERCENT, const float MEAN, const float STD) {
	noisedInstancesId.clear();

	const int NUM_INSTANCES = getNumInstances();
	unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<float> gaussian(MEAN, STD);

	NUM_NOISED_INSTANCES = (const int)(NUM_INSTANCES * RATIO_PERCENT);
	
	int* pos = new int[NUM_NOISED_INSTANCES];

	Rnd::randSeq<int>(pos, 0, NUM_INSTANCES, NUM_NOISED_INSTANCES, false);

	for (int i = 0; i < NUM_NOISED_INSTANCES; i++) {
		for (int j = 0; j < DIM_DATA_VECTOR; j++) {
			instances[pos[i]]->dataVector[j] += gaussian(generator);
		}

		noisedInstancesId.push_back(instances[pos[i]]->id);
	}

	Arr::sort<int>(&noisedInstancesId);

	delete[] pos;
}

int Dataset::idToIndex(const int ID) {
	const int NUM_INSTANCES = (const int)instances.size();

	for (int i = 0; i < NUM_INSTANCES; i++) {
		if (instances[i]->id == ID) {
			return i;
		}
	}

	return -1;
}

int Dataset::indexToId(const int INDEX) {
	return instances[INDEX]->id;
}

void Dataset::splitTestDataset(const float RATIO) {
	const int NUM_TEST_INSTANCES = getNumInstances() * RATIO;
	Dataset* datasets = new Dataset(getType(), getDimDataVector(), getDimTargetVector(), getNumClasses());
	int* randSeq = new int[NUM_TEST_INSTANCES];

	Rnd::randSeq<int>(randSeq, 0, getNumInstances(), NUM_TEST_INSTANCES, false);
	Arr::sort<int>(randSeq, NUM_TEST_INSTANCES, true);

	for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
		testInstances.push_back(new Instance(instances[randSeq[i]]));
		instances.erase(instances.begin() + randSeq[i]);
	}

	shuffle();
}

void Dataset::print() {
	const int NUM_INSTANCES = (const int)instances.size();
	const int NUM_TEST_INSTANCES = (const int)testInstances.size();

	cout << "Training dataset: " << endl;
	for (int i = 0; i < NUM_INSTANCES; i++) {
		cout << Arr::toString(instances[i]->dataVector, DIM_DATA_VECTOR);
		
		if (TYPE == LABELLED) {
			cout << " " << Arr::toString(instances[i]->targetVector, DIM_TARGET_VECTOR) << endl;
		}
		else if (TYPE == UNLABELLED) {
			cout << endl;
		}
	}

	if (NUM_TEST_INSTANCES == 0) {
		cout << endl << "Test dataset: None" << endl;
	}
	else {
		cout << endl << "Test dataset: " << endl;
		for (int i = 0; i < NUM_TEST_INSTANCES; i++) {
			cout << Arr::toString(testInstances[i]->dataVector, DIM_DATA_VECTOR);

			if (TYPE == LABELLED) {
				cout << " " << Arr::toString(testInstances[i]->targetVector, DIM_TARGET_VECTOR) << endl;
			}
			else if (TYPE == UNLABELLED) {
				cout << endl;
			}
		}
	}
}

void Dataset::save(const string FILE_NAME) {
	const int NUM_INSTANCES = getNumInstances();
	ofstream dataFile(FILE_NAME);

	/*
	dataFile << NUM_INSTANCES << endl;
	dataFile << DIM_DATA_VECTOR << endl;
	dataFile << DIM_TARGET_VECTOR << endl;
	dataFile << NUM_CLASSES << endl;
	*/

	for (int i = 0; i < NUM_INSTANCES; i++) {
		dataFile << vectorToString(instances[i]->dataVector, DIM_DATA_VECTOR);

		if (DIM_TARGET_VECTOR > 0) {
			dataFile << "," + vectorToString(instances[i]->targetVector, DIM_TARGET_VECTOR);
		}

		if (i < NUM_INSTANCES - 1) {
			dataFile << endl;
		}
	}

	dataFile.close();
}
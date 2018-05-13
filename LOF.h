#pragma once

#include "LibNGS\Dataset.h"
#include "LibNGS\Arr.h"
#include "LibNGS\Mem.h"
#include "LibNGS\math.h"
#include "LOFConfiguration.h"

#define DIST_SAME_INSTANCE		0.1

class LOF {
protected:
	int K = 0;
	int NUM_DATA_POINTS = 0;
	int DIM_DATA_POINT = 0;
	float THRESHOLD = 0;

	float** reachDist = NULL;
	float* lrd = NULL;

	void clear(const bool CLEAR_OUTLIERS);
	void initialize(Dataset& dataset, const bool CLEAR_OUTLIERS);
	void createDistMatrix(Dataset& dataset);
	void createKNNMatrix();
	void createKNNMatrix(const int INDEX);
	float computeKDist(const int INDEX);
	float computeReachDist(const int TARGET_INDEX, const int ANOTHER_INDEX);
	float computeLRD(const int INDEX);
	float computeLOF(const int INDEX);

public:
	float** distMatrix = NULL;
	int** knnMatrix = NULL;
	float* kDist = NULL;
	float* lof = NULL;
	vector<int> outliersId;

	LOF(LOFConfiguration& conf);
	~LOF();
	int getNumOutliers();
	void run(Dataset& dataset, const bool CLEAR_OUTLIERS = true, const bool CHECK_OUTLIER = true);
};

void LOF::clear(const bool CLEAR_OUTLIERS) {
	delete[] kDist;
	delete[] lrd;
	delete[] lof;
	Mem::delete2D<float>(distMatrix, NUM_DATA_POINTS);
	Mem::delete2D<int>(knnMatrix, NUM_DATA_POINTS);
	Mem::delete2D<float>(reachDist, NUM_DATA_POINTS);

	if (CLEAR_OUTLIERS) {
		outliersId.clear();
	}
}

void LOF::initialize(Dataset& dataset, const bool CLEAR_OUTLIERS) {
	clear(CLEAR_OUTLIERS);

	DIM_DATA_POINT = dataset.getDimDataVector();
	NUM_DATA_POINTS = dataset.getNumInstances();

	kDist = new float[NUM_DATA_POINTS];
	lrd = new float[NUM_DATA_POINTS];
	lof = new float[NUM_DATA_POINTS];

	Mem::alloc2D<float>(distMatrix, NUM_DATA_POINTS, NUM_DATA_POINTS);
	Mem::alloc2D<int>(knnMatrix, NUM_DATA_POINTS, this->K);
	Mem::alloc2D<float>(reachDist, NUM_DATA_POINTS, NUM_DATA_POINTS);

	Arr::set<float>(distMatrix, NUM_DATA_POINTS, NUM_DATA_POINTS, -1);
	Arr::set<int>(knnMatrix, NUM_DATA_POINTS, this->K, -1);
}

void LOF::createDistMatrix(Dataset& dataset) {
	float** dataVectors = NULL;

	dataset.getDataVectors(dataVectors);
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		for (int j = 0; j < NUM_DATA_POINTS; j++) {
			if (i == j) {
				distMatrix[i][j] = 0;
				continue;
			}

			if (distMatrix[j][i] == -1.0f) {
				distMatrix[i][j] = Math::eucDist<float>(dataVectors[i], dataVectors[j], DIM_DATA_POINT);
				if (distMatrix[i][j] == 0) {
					distMatrix[i][j] = (float)DIST_SAME_INSTANCE;
				}
			}
			else {
				distMatrix[i][j] = distMatrix[j][i];
			}
		}
	}

	delete[] dataVectors;
}

void LOF::createKNNMatrix() {
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		for (int j = 0; j < NUM_DATA_POINTS; j++) {
			if (j == i) {
				continue;
			}

			for (int k = 0; k < K; k++) {
				if (knnMatrix[i][k] == -1) {
					knnMatrix[i][k] = j;
					break;
				}
				else {
					if (distMatrix[i][j] < distMatrix[i][knnMatrix[i][k]]) {
						for (int q = K - 2; q >= k; q--) {
							knnMatrix[i][q + 1] = knnMatrix[i][q];
						}
						knnMatrix[i][k] = j;
						break;
					}
				}
			}
		}
	}
}

void LOF::createKNNMatrix(const int INDEX) {
	for (int j = 0; j < NUM_DATA_POINTS; j++) {
		if (j == INDEX) {
			continue;
		}

		for (int k = 0; k < K; k++) {
			if (knnMatrix[INDEX][k] == -1) {
				knnMatrix[INDEX][k] = j;
				break;
			}
			else {
				if (distMatrix[INDEX][j] < distMatrix[INDEX][knnMatrix[INDEX][k]]) {
					for (int q = K - 2; q >= k; q--) {
						knnMatrix[INDEX][q + 1] = knnMatrix[INDEX][q];
					}
					knnMatrix[INDEX][k] = j;
					break;
				}
			}
		}
	}
}

float LOF::computeKDist(const int INDEX) {
	return distMatrix[INDEX][knnMatrix[INDEX][K - 1]];
}

float LOF::computeReachDist(const int TARGET_INDEX, const int ANOTHER_INDEX) {
	if (kDist[ANOTHER_INDEX] > distMatrix[TARGET_INDEX][ANOTHER_INDEX]) {
		return kDist[ANOTHER_INDEX];
	}
	else {
		return distMatrix[TARGET_INDEX][ANOTHER_INDEX];
	}
}

float LOF::computeLRD(const int INDEX) {
	float sumReachDistances = 0;

	for (int i = 0; i < K; i++) {
		sumReachDistances += reachDist[INDEX][knnMatrix[INDEX][i]];
	}

	return K / sumReachDistances;
}

float LOF::computeLOF(const int INDEX) {
	float sumLRD = 0;

	for (int i = 0; i < K; i++) {
		sumLRD += lrd[knnMatrix[INDEX][i]];
	}

	return (sumLRD / (float)K) / lrd[INDEX];
}

LOF::LOF(LOFConfiguration& conf) {
	this->DIM_DATA_POINT = conf.DIM_DATA_POINT;
	this->K = conf.K;
	this->THRESHOLD = conf.THRESHOLD;
}

LOF::~LOF() {
	clear(true);
}

int LOF::getNumOutliers() {
	return (int)outliersId.size();
}

void LOF::run(Dataset& dataset, const bool CLEAR_OUTLIERS, const bool CHECK_OUTLIER) {
	initialize(dataset, CLEAR_OUTLIERS);

	createDistMatrix(dataset);
	createKNNMatrix();

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		kDist[i] = computeKDist(i);
	}

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		for (int j = 0; j < K; j++) {
			reachDist[i][knnMatrix[i][j]] = computeReachDist(i, knnMatrix[i][j]);
		}
	}

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		lrd[i] = computeLRD(i);
	}

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		lof[i] = computeLOF(i);
	}

	if (CHECK_OUTLIER) {
		for (int i = 0; i < NUM_DATA_POINTS; i++) {
			if (lof[i] > THRESHOLD) {
				outliersId.push_back(dataset.instances[i]->id);
			}
		}
	}
}
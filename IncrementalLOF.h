#pragma once

#include "LibNGS\Dataset.h"
#include "LibNGS\Math.h"
#include "LOF.h"

#define INITIAL_BUFFER_SIZE_FACTOR		4

class IncrementalLOF : public LOF {
protected:
	int INITIAL_BUFFER_SIZE = 0;
	bool ENABLE_SKIP = false;

	Dataset dataset;
	bool isInitialPhase = true;
	float* lastOutlier = NULL;

	void reset(Instance* instance);
	void computeAddedDistMatrix();

public:
	IncrementalLOF(LOFConfiguration& conf);
	void insertion(Instance* instance, const bool DSiLOF_A);
};

void IncrementalLOF::reset(Instance* instance) {
	const int NUM_PREV_DATA_VECTORS = NUM_DATA_POINTS;
	float** tempDistMatrix = NULL;
	int** tempknnMatrix = NULL;
	float** tempReachDist = NULL;
	float* tempKDist = NULL;
	float* tempLRD = NULL;
	float* tempLOF = NULL;

	NUM_DATA_POINTS = dataset.getNumInstances();

	Mem::alloc2D<float>(tempDistMatrix, NUM_DATA_POINTS + 1, NUM_DATA_POINTS + 1);
	Mem::alloc2D<int>(tempknnMatrix, NUM_DATA_POINTS + 1, K);
	Mem::alloc2D<float>(tempReachDist, NUM_DATA_POINTS + 1, NUM_DATA_POINTS + 1);
	tempKDist = new float[NUM_DATA_POINTS + 1];
	tempLRD = new float[NUM_DATA_POINTS + 1];
	tempLOF = new float[NUM_DATA_POINTS + 1];

	Arr::copy<float>(tempDistMatrix, distMatrix, NUM_DATA_POINTS, NUM_DATA_POINTS);
	Arr::copy<float>(tempReachDist, reachDist, NUM_DATA_POINTS, NUM_DATA_POINTS);
	Arr::copy<float>(tempKDist, kDist, NUM_DATA_POINTS);
	Arr::set<int>(tempknnMatrix, NUM_DATA_POINTS + 1, K, -1);
	Arr::copy<int>(tempknnMatrix, knnMatrix, NUM_DATA_POINTS, K);

	Mem::delete2D<float>(distMatrix, NUM_PREV_DATA_VECTORS);
	Mem::delete2D<int>(knnMatrix, NUM_PREV_DATA_VECTORS);
	Mem::delete2D<float>(reachDist, NUM_PREV_DATA_VECTORS);
	delete[] kDist;
	delete[] lrd;
	delete[] lof;

	distMatrix = tempDistMatrix;
	knnMatrix = tempknnMatrix;
	reachDist = tempReachDist;
	kDist = tempKDist;
	lrd = tempLRD;
	lof = tempLOF;

	dataset.addInstanceCopy(instance);
	NUM_DATA_POINTS = dataset.getNumInstances();
}

void IncrementalLOF::computeAddedDistMatrix() {
	float** dataVectors = NULL;

	dataset.getDataVectors(dataVectors);
	distMatrix[NUM_DATA_POINTS - 1][NUM_DATA_POINTS - 1] = 0;

	for (int i = 0; i < NUM_DATA_POINTS - 1; i++) {
		distMatrix[NUM_DATA_POINTS - 1][i] = Math::eucDist<float>(dataVectors[NUM_DATA_POINTS - 1], dataVectors[i], DIM_DATA_POINT);
		distMatrix[i][NUM_DATA_POINTS - 1] = distMatrix[NUM_DATA_POINTS - 1][i];
	}

	delete[] dataVectors;
}

IncrementalLOF::IncrementalLOF(LOFConfiguration& conf) : LOF(conf) {
	this->THRESHOLD = conf.THRESHOLD;
	this->INITIAL_BUFFER_SIZE = K * INITIAL_BUFFER_SIZE_FACTOR;

	dataset.initialize(Dataset::UNLABELLED, DIM_DATA_POINT, 0, 0);
}

void IncrementalLOF::insertion(Instance* instance, const bool DSiLOF_A) {
	int neighborIndex = -1;
	vector<int> updateQueue;

	if (isInitialPhase && NUM_DATA_POINTS < INITIAL_BUFFER_SIZE) {
		dataset.addInstanceCopy(instance);
		NUM_DATA_POINTS++;

		if (NUM_DATA_POINTS == INITIAL_BUFFER_SIZE) {
			run(dataset, true);
			isInitialPhase = false;
			if (outliersId.size() > 0) {
				lastOutlier = dataset.instances[outliersId[outliersId.size() - 1]]->dataVector;
			}
		}

		return;
	}

	if (DSiLOF_A) {
		if (ENABLE_SKIP) {
			const float outlierDist = Math::eucDist<float>(instance->dataVector, lastOutlier, DIM_DATA_POINT);
			float avgDistFirstNN = 0;

			for (int i = 0; i < NUM_DATA_POINTS; i++) {
				avgDistFirstNN += distMatrix[i][knnMatrix[i][0]];
			}
			avgDistFirstNN /= (float)NUM_DATA_POINTS;

			if (outlierDist < avgDistFirstNN) {
				outliersId.push_back(instance->id);
				return;
			}
			else {
				ENABLE_SKIP = false;
			}
		}
	}

	reset(instance);
	computeAddedDistMatrix();
	createKNNMatrix(NUM_DATA_POINTS - 1);

	kDist[NUM_DATA_POINTS - 1] = computeKDist(NUM_DATA_POINTS - 1);
	for (int i = 0; i < K; i++) {
		neighborIndex = knnMatrix[NUM_DATA_POINTS - 1][i];
		reachDist[NUM_DATA_POINTS - 1][neighborIndex] = computeReachDist(NUM_DATA_POINTS - 1, neighborIndex);
	}

	for (int i = 0; i < NUM_DATA_POINTS - 1; i++) {
		for (int j = 0; j < K; j++) {
			if (knnMatrix[i][j] == NUM_DATA_POINTS - 1) {
				updateQueue.push_back(i);
				break;
			}
		}
	}

	const int SIZE_UPDATE_QUEUE = (const int)updateQueue.size();

	for (int i = 0; i < SIZE_UPDATE_QUEUE; i++) {
		kDist[updateQueue[i]] = computeKDist(updateQueue[i]);
		reachDist[updateQueue[i]][NUM_DATA_POINTS - 1] = computeReachDist(updateQueue[i], NUM_DATA_POINTS - 1);
	}
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		for (int j = 0; j < K; j++) {
			for (int k = 0; k < SIZE_UPDATE_QUEUE; k++) {
				if (knnMatrix[i][j] == updateQueue[k]) {
					reachDist[i][j] = computeReachDist(i, j);
					break;
				}
			}
		}
	}

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		lrd[i] = computeLRD(i);
	}

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		lof[i] = computeLOF(i);
	}

	if (lof[NUM_DATA_POINTS - 1] > THRESHOLD) {
		outliersId.push_back(instance->id);

		if (DSiLOF_A) {
			lastOutlier = dataset.instances[NUM_DATA_POINTS - 1]->dataVector;
			ENABLE_SKIP = true;
		}
	}
}
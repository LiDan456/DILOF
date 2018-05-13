#pragma once

#include <iostream>
#include <queue>
#include <algorithm>
#include "LibNGS\Arr.h"
#include "IncrementalLOF.h"

class DILOF: public IncrementalLOF {
protected:
	int W = 0;
	int H_W = 0;
	int Q_W = 0;

	void summarize();
	void selectOptimalInstances();
	int getKthNNIndex(const int INDEX, int* y);
	float getKthNNDist(const int INDEX, float* tempY);
	float getApproximatedKthNNDist(const int INDEX, const float SUM_NN_LOF, const float SUM_LOF);
	void project(int*& y, float* tempY);

public:
	DILOF(LOFConfiguration& conf);
	void insertion(Instance* instance);
};

void DILOF::summarize() {
	selectOptimalInstances();
	run(dataset, false, false);
}

void DILOF::selectOptimalInstances() {
	const int MAX_ITERATION = 20;
	float LEARNING_RATE = 0.3f;

	int* y = new int[H_W];
	float* tempY = new float[H_W];
	float* approxRho = new float[H_W];
	float* gradients = new float[H_W];
	float* sumNNLOF = new float[H_W];
	float* expSigLOF = new float[H_W];
	int* kthNNs = new int[H_W];
	float sumY = 0;
	float sumLOF = 0;
	float sumDist = 0;
	float avgSumNNLOF = 0;
	int kthNN = -1;

	Arr::set<float>(tempY, H_W, 0.5);
	Arr::set<float>(sumNNLOF, H_W, 0);

	for (int i = 0; i < H_W; i++) {
		expSigLOF[i] = exp(Math::sigmoid(lof[i]));
	}

	for (int i = 0; i < H_W; i++) {
		for (int k = 0; k < K; k++) {
			if (knnMatrix[i][k] < H_W) {
				sumNNLOF[i] += expSigLOF[knnMatrix[i][k]];
			}
		}
		avgSumNNLOF += sumNNLOF[i];

		sumLOF += expSigLOF[i];
	}
	avgSumNNLOF /= (float)H_W;

	for (int i = 0; i < H_W; i++) {
		approxRho[i] = getApproximatedKthNNDist(i, sumNNLOF[i], sumLOF);
		kthNN = knnMatrix[i][K - 1];
		kthNNs[i] = kthNN;

		if (sumNNLOF[i] > avgSumNNLOF) {
			for (int j = 0; j < H_W; j++) {
				if (distMatrix[i][j] > distMatrix[i][kthNN] && distMatrix[i][j] < 2 * Math::sigmoid(lof[i]) * distMatrix[i][kthNN]) {
					kthNNs[i] = j;
					break;
				}
			}
		}
	}

	for (int c = 0; c < MAX_ITERATION; c++) {
		LEARNING_RATE *= 0.95;
		sumY = 0;

		for (int i = 0; i < H_W; i++) {
			sumY += tempY[i];
		}

		for (int i = 0; i < H_W; i++) {
			sumDist = 0;

			for (int n = 0; n < H_W; n++) {
				if (kthNNs[n] == i) {
					sumDist += tempY[n] * (distMatrix[n][i] / distMatrix[n][knnMatrix[n][K - 1]]);
				}
			}

			gradients[i] = (sumDist + approxRho[i]) - exp(lof[i]) + 0.001 * (sumY - Q_W);

			if (tempY[i] > 1) {
				gradients[i] += 2 * (tempY[i] - 1);
			}
			else if (tempY[i] < 0) {
				gradients[i] += 2 * tempY[i];
			}
		}

		for (int i = 0; i < H_W; i++) {
			tempY[i] = tempY[i] - LEARNING_RATE * gradients[i];
		}
	}

	project(y, tempY);

	for (int i = H_W - 1; i > -1; i--) {
		if (y[i] == 0) {
			dataset.removeInstance(i);
		}
	}

	delete[] y;
	delete[] tempY;
	delete[] approxRho;
	delete[] gradients;
	delete[] sumNNLOF;
	delete[] expSigLOF;
	delete[] kthNNs;
}

int DILOF::getKthNNIndex(const int INDEX, int* y) {
	int* nn = new int[K];
	int kthNNIndex = -1;

	Arr::set<int>(nn, K, -1);

	for (int i = 0; i < H_W; i++) {
		if (i == INDEX) {
			continue;
		}

		for (int j = 0; j < K; j++) {
			if (nn[j] == -1) {
				nn[j] = j;
				break;
			}
			else {
				if (y[i] * distMatrix[INDEX][i] < y[i] * distMatrix[INDEX][knnMatrix[INDEX][j]]) {
					for (int k = j - 2; k >= j; k--) {
						nn[k + 1] = nn[k];
					}
					nn[j] = i;
					break;
				}
			}
		}
	}
	kthNNIndex = nn[K - 1];

	delete[] nn;

	return kthNNIndex;
}

float DILOF::getKthNNDist(const int INDEX, float* tempY) {
	int* nn = new int[K];
	int* y = new int[H_W];

	project(y, tempY);
	Arr::set<int>(nn, K, -1);

	for (int i = 0; i < H_W; i++) {
		if (i == INDEX) {
			continue;
		}

		for (int j = 0; j < K; j++) {
			if (nn[j] == -1) {
				nn[j] = j;
				break;
			}
			else {
				if (y[i] * distMatrix[INDEX][i] < y[i] * distMatrix[INDEX][knnMatrix[INDEX][j]]) {
					for (int k = j - 2; k >= j; k--) {
						nn[k + 1] = nn[k];
					}
					nn[j] = i;
					break;
				}
			}
		}
	}
	
	float dist = distMatrix[INDEX][nn[K - 1]];

	delete[] nn;
	delete[] y;

	return dist;
}

float DILOF::getApproximatedKthNNDist(const int INDEX, const float SUM_NN_LOF, const float SUM_LOF) {
	float kthNNDistance = distMatrix[INDEX][knnMatrix[INDEX][K - 1]];
	float maxDistance = distMatrix[INDEX][0];

	for (int i = 1; i < H_W; i++) {
		if (distMatrix[INDEX][i] > maxDistance) {
			maxDistance = distMatrix[INDEX][i];
		}
	}

	return (kthNNDistance + (SUM_NN_LOF / SUM_LOF) * (maxDistance - kthNNDistance)) / distMatrix[INDEX][knnMatrix[INDEX][K - 1]];
}

void DILOF::project(int*& y, float* tempY) {
	pair<float, int>* pairs = new pair<float, int>[H_W];

	for (int i = 0; i < H_W; i++) {
		pairs[i] = make_pair(tempY[i], i);
	}

	sort(pairs, pairs + H_W, [](pair<float, int> const& a, pair<float, int> const& b) -> bool { return a.first > b.first; });

	for (int i = 0; i < Q_W; i++) {
		y[pairs[i].second] = 1;
	}

	for (int i = Q_W; i < H_W; i++) {
		y[pairs[i].second] = 0;
	}

	delete[] pairs;
}

DILOF::DILOF(LOFConfiguration& conf) : IncrementalLOF(conf) {
	this->W = conf.MAX_NUM_INSTANCES;
	this->H_W = this->W / 2.0f;
	this->INITIAL_BUFFER_SIZE = this->H_W;
	this->Q_W = this->H_W / 2.0f;
}

void DILOF::insertion(Instance* instance) {
	IncrementalLOF::insertion(instance, true);

	if (dataset.getNumInstances() == W) {
		summarize();
	}
}
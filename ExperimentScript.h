#pragma once

#pragma once

#include <iostream>
#include <fstream>

#include "LibNGS\Time.h"
#include "LibNGS\Performance.h"
#include "DatasetConfiguration.h"
#include "LOFConfiguration.h"
#include "IncrementalLOF.h"
#include "DILOF.h"
#include "DILOF_NS.h"

using namespace std;

void expScript(const string RESULT_FILE_NAME,
	DatasetConfiguration* dConf,
	const int K,
	const int* windows,
	const int NUM_WINDOWS,
	const float* thresholds,
	const int NUM_THRESHOLDS,
	const bool RUN_DILOF) {

	int NUM_INSTANCES = 0;
	int NUM_OUTLIERS = 0;

	ofstream results(RESULT_FILE_NAME);
	Dataset dataset(dConf->DATASET_TYPE, dConf->DATA_FILE_NAME, dConf->DELIMITER);
	float* AUC = new float[NUM_WINDOWS];
	float* execTime = new float[NUM_WINDOWS];
	float* TPR = new float[NUM_THRESHOLDS];
	float* FPR = new float[NUM_THRESHOLDS];
	int* trueOutliers = NULL;

	NUM_INSTANCES = dataset.getNumInstances();

	Arr::set<float>(AUC, NUM_WINDOWS, 0);
	Arr::set<float>(execTime, NUM_WINDOWS, 0);
	if (dConf->NORMALIZE) {
		dataset.normalize();
	}

	if (dConf->TYPE_NOISE == DatasetConfiguration::TYPE_NOISE_CLASS) {
		NUM_OUTLIERS = dataset.getInstancesIdByClass(trueOutliers, dConf->NOISE_CLASS_INDEX);
	}
	else {
		if (dConf->TYPE_NOISE == DatasetConfiguration::TYPE_NOISE_UNIFORM) {
			dataset.addUniformNoise(dConf->NOISE_RATIO, dConf->NOISE_MIN, dConf->NOISE_MAX);
			NUM_OUTLIERS = dataset.getNumNoisedInstances();
			trueOutliers = new int[NUM_OUTLIERS];
			Arr::copy<int>(trueOutliers, &dataset.noisedInstancesId[0], NUM_OUTLIERS);
		}
	}	

	for (int j = 0; j < NUM_WINDOWS; j++) {
		Arr::set<float>(TPR, NUM_THRESHOLDS, 0);
		Arr::set<float>(FPR, NUM_THRESHOLDS, 0);

		for (int k = 0; k < NUM_THRESHOLDS; k++) {
			LOFConfiguration lConf(dataset.getDimDataVector(), K, thresholds[k], windows[j]);

			cout << "---Window(" << windows[j] << "), Threshold(" << thresholds[k] << ")" << endl;

			if (RUN_DILOF) {
				// Run DILOF.
				DILOF dilof(lConf);
				Time::startTimeChecking();
				for (int q = 0; q < NUM_INSTANCES; q++) {
					dilof.insertion(dataset.instances[q]);
				}
				execTime[j] += Time::stopTimeChecking(Time::TIME_SECOND);

				if (dilof.getNumOutliers() > 0) {
					TPR[k] = Performance::TPR(&dilof.outliersId[0], dilof.getNumOutliers(), &trueOutliers[0], NUM_OUTLIERS, NUM_INSTANCES);
					FPR[k] = Performance::FPR(&dilof.outliersId[0], dilof.getNumOutliers(), &trueOutliers[0], NUM_OUTLIERS, NUM_INSTANCES);
				}
			}
			else {
				// Run DILOF_NC.
				DILOF_NS dilofns(lConf);
				Time::startTimeChecking();
				for (int q = 0; q < NUM_INSTANCES; q++) {
					dilofns.insertion(dataset.instances[q]);
				}
				execTime[j] += Time::stopTimeChecking(Time::TIME_SECOND);

				if (dilofns.getNumOutliers() > 0) {
					TPR[k] = Performance::TPR(&dilofns.outliersId[0], dilofns.getNumOutliers(), &trueOutliers[0], NUM_OUTLIERS, NUM_INSTANCES);
					FPR[k] = Performance::FPR(&dilofns.outliersId[0], dilofns.getNumOutliers(), &trueOutliers[0], NUM_OUTLIERS, NUM_INSTANCES);
				}
			}
		}

		AUC[j] = Math::trapz<float>(FPR, TPR, NUM_THRESHOLDS);
		cout << "AUC: " << AUC[j] << endl;
		cout << "Execution time: " << execTime[j] / (float)NUM_THRESHOLDS << endl;
	}

	delete[] AUC;
	delete[] execTime;
	delete[] TPR;
	delete[] FPR;
	delete[] trueOutliers;
}
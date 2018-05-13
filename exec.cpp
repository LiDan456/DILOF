#include "DatasetConfiguration.h"
#include "ExperimentScript.h"

#define NUM_SMALL_WINDOWS		 6
#define NUM_LARGE_WINDOWS		 4
#define NUM_THRESHOLDS	10

void main() {
	int smallWindows[NUM_SMALL_WINDOWS] = { 100, 120, 140, 160, 180, 200 };
	int largeWindows[NUM_LARGE_WINDOWS] = { 100, 200, 300, 400 };
	float thresholds[NUM_THRESHOLDS] = { 0.1f, 1.0f, 1.1f, 1.15f, 1.2f, 1.3f, 1.4f, 1.6f, 2.0f, 3.0f };
	DatasetConfiguration dConfVowel(DatasetConfiguration::LABELLED, "Dataset/vowel2.txt", ",", false, false);
	DatasetConfiguration dConfPendigit(DatasetConfiguration::LABELLED, "Dataset/pendigit.txt", ",", false, true);
	DatasetConfiguration dConfSMTP(DatasetConfiguration::LABELLED, "Dataset/smtp.txt", ",", false, false);
	DatasetConfiguration dConfHTTP(DatasetConfiguration::LABELLED, "Dataset/http.txt", ",", false, false);

	dConfVowel.setNoiseClass(1);
	dConfPendigit.addUniformNoise(0.05f, 0, 0.5);
	dConfSMTP.setNoiseClass(1);
	dConfHTTP.setNoiseClass(1);

	// The UCI Vowel dataset.
	cout << "1. UCI Vowel dataset" << endl;
	expScript("Results/auc_vowel.txt", &dConfVowel, 19, smallWindows, NUM_SMALL_WINDOWS, thresholds, NUM_THRESHOLDS, false);

	// The UCI Pendigit dataset.
	//	cout << "2. UCI Pendigit dataset" << endl;
	//	expScript("Results/auc_pen.txt", &dConfPendigit, 8, smallWindows, NUM_SMALL_WINDOWS, thresholds, NUM_THRESHOLDS, true);

	// The KDD Cup 99 smtp dataset.
	//	cout << "3. KDD Cup 99 smtp dataset" << endl;
	//	expScript("Results/auc_smtp.txt", &dConfSMTP, 8, largeWindows, NUM_LARGE_WINDOWS, thresholds, NUM_THRESHOLDS, true);

	// The KDD Cup 99 http dataset.
	//	cout << "4. KDD Cup 99 http dataset" << endl;
	//	expScript("Results/auc_http8.txt", &dConfHTTP, 8, largeWindows, NUM_LARGE_WINDOWS, thresholds, NUM_THRESHOLDS, true);
}
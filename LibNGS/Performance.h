#pragma once

class Performance {
public:
	static const int TYPE_TPR = 0;
	static const int TYPE_FPR = 1;
	static const int TYPE_AUC = 2;
	static const int TYPE_PRECISION = 3;

	template <typename T> static float TPR(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES);
	template <typename T> static float FPR(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES);
	template <typename T> static float AUC(const T* FPR, const T* TPR, const int NUM_POINTS);
	template <typename T> static float PPV(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES);
	template <typename T> static float F1(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES);
};

template <typename T>
float Performance::TPR(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES) {
	// Compute true positive rate: TP / P.

	int TP = 0;

	for (int i = 0; i < NUM_RESULTS; i++) {
		for (int j = 0; j < NUM_LABELS; j++) {
			if (results[i] == labels[j]) {
				TP++;
				break;
			}
		}
	}

	return TP / (float)NUM_LABELS;
}

template <typename T>
float Performance::FPR(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES) {
	// Compute true positive rate: FP / N.

	int TP = 0;
	int FP = 0;

	for (int i = 0; i < NUM_RESULTS; i++) {
		for (int j = 0; j < NUM_LABELS; j++) {
			if (results[i] == labels[j]) {
				TP++;
				break;
			}
		}
	}

	return (NUM_RESULTS - TP) / (float)(NUM_INSTANCES - NUM_LABELS);
}

template <typename T>
float Performance::AUC(const T* FPR, const T* TPR, const int NUM_POINTS) {
	// Compute the Area Under the Curve (AUC).

	return Math::trapz<T>(FPR, TPR, NUM_POINTS);
}

template <typename T>
float Performance::PPV(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES) {
	// Compute true positive rate: TP / (TP + FP).

	int TP = 0;
	int FP = 0;

	for (int i = 0; i < NUM_RESULTS; i++) {
		for (int j = 0; j < NUM_LABELS; j++) {
			if (results[i] == labels[j]) {
				TP++;
				break;
			}
		}
	}

	FP = NUM_RESULTS - TP;

	return TP / (float)(TP + FP);
}

template <typename T>
float Performance::F1(T* results, const int NUM_RESULTS, T* labels, const int NUM_LABELS, const int NUM_INSTANCES) {
	// Compute F1 score: 2 * TP / (2 * TP + FP + FN)

	int TP = 0;
	int FP = 0;
	int FN = 0;

	for (int i = 0; i < NUM_RESULTS; i++) {
		for (int j = 0; j < NUM_LABELS; j++) {
			if (results[i] == labels[j]) {
				TP++;
				break;
			}
		}
	}

	FP = NUM_RESULTS - TP;
	FN = NUM_LABELS - TP;

	return 2 * TP / (float)(2 * TP + FP + FN);
}
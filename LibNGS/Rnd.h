#pragma once

#include <random>
#include <vector>

using namespace std;

class Rnd {
public:
	template <typename T> static T rand(const T MIN, const T MAX);
	template <typename T> static void randSeq(T*& seq, const T MIN, const T MAX, const int LEN_SEQ, const bool DUPLICABLE);
	template <typename T> static T normal(const T MEAN, const T STD);
	static int rouletteSelection(float* PROBS, const int NUM_INSTANCES, const float SUM_PROBS);
};

template <typename T> T Rnd::rand(const T MIN, const T MAX) {
	random_device rd;
	mt19937 eng(rd());

	if (typeid(T) == typeid(int)) {
		uniform_int_distribution<> dist(MIN, MAX - 1);

		return (T)dist(eng);
	}
	else {
		uniform_real_distribution<> dist(MIN, MAX);

		return (T)dist(eng);
	}
}

template <typename T> void Rnd::randSeq(T*& seq, const T MIN, const T MAX, const int LEN_SEQ, const bool DUPLICABLE) {
	random_device rd;
	mt19937 eng(rd());

	if (DUPLICABLE) {
		if (typeid(T) == typeid(int)) {
			uniform_int_distribution<> dist(MIN, MAX - 1);

			for (int i = 0; i < LEN_SEQ; i++) {
				seq[i] = dist(eng);
			}
		}
		else {
			uniform_real_distribution<> dist(MIN, MAX);

			for (int i = 0; i < LEN_SEQ; i++) {
				seq[i] = (float)dist(eng);
			}
		}
	}
	else {
		if (typeid(T) == typeid(int)) {
			vector<int> numbers;
			int selectedIndex = 0;

			for (int i = MIN; i < MAX; i++) {
				numbers.push_back(i);
			}

			for (int i = 0; i < LEN_SEQ; i++) {
				uniform_int_distribution<> dist(0, (const int)numbers.size() - 1);

				selectedIndex = dist(eng);
				seq[i] = numbers[selectedIndex];
				numbers.erase(numbers.begin() + selectedIndex);
			}
		}
		else {
			uniform_real_distribution<> dist(MIN, MAX);
			float temp;

			for (int i = 0; i < LEN_SEQ; i++) {
				temp = (float)dist(eng);

				for (int j = 0; j < i; j++) {
					if (temp == seq[j]) {
						temp = (float)dist(eng);
						j = 0;
					}
				}

				seq[i] = (float)dist(eng);
			}
		}
	}
}

template <typename T> T Rnd::normal(const T MEAN, const T STD) {
	random_device rd;
	mt19937 eng(rd());
	normal_distribution<T> dist(MEAN, STD);

	return (T)dist(rd);
}

int Rnd::rouletteSelection(float* PROBS, const int NUM_INSTANCES, const float SUM_PROBS) {
	const float position = Rnd::rand<float>(0, 1) * SUM_PROBS;
	float sum = 0;

	for (int i = 0; i < NUM_INSTANCES; i++) {
		sum += PROBS[i];

		if (position < sum) {
			return i;
		}
	}

	return -1;
}
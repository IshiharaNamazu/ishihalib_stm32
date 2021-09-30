#pragma once

#include <algorithm>
#include <vector>

class LowPassFilter {
	float K;
	float prev;

  public:
	LowPassFilter(float Gain) : K(Gain), prev(0.) {
	}

	float calc(float data) {  //O(1)
		return prev = data * K + (1 - K) * prev;
	}
};

class AverageFilter {
	int n_sample;
	std::vector<float> data;
	float sum = 0;
	int cnt = 0;

  public:
	AverageFilter(int num_of_sampling) : n_sample(num_of_sampling), data(num_of_sampling, 0.0) {
	}

	float calc(float new_data) {  //O(1)
		sum -= data[cnt];
		sum += new_data;

		data[cnt] = new_data;
		cnt++;
		cnt %= n_sample;

		return (sum / n_sample);
	}
};

class MedianFilter {
	int n_sample;
	std::vector<float> data;
	int cnt = 0;

  public:
	MedianFilter(int num_of_sampling) : n_sample(num_of_sampling), data(num_of_sampling, 0.0) {
	}

	float calc(float new_data) {  //O(NlogN)
		data[cnt] = new_data;

		cnt++;
		cnt %= n_sample;

		std::vector<float> sorted_data(n_sample);
		for (int i = 0; i < n_sample; i++) sorted_data[i] = data[i];  //copy
		std::sort(sorted_data.begin(), sorted_data.end());			  //sort
		return sorted_data[(n_sample + 1) / 2];						  //median
	}
};
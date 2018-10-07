#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct individual {
	vector<double> x;
	vector<double> s;
	double f_value;
};

#define SIZE_ERROR 2

template<typename T>
vector<T> operator*(vector<T> v1, vector<T> v2) {
	if (v1.size() != v2.size()) {
		cout << "Size " << v1.size()
			 << " and " << v2.size()
			 << " are incompatible" << endl;
		exit(SIZE_ERROR);
	}
	vector<T> res;
	for (int i = 0; i < v1.size(); i++) {
		res.push_back(v1[i] * v2[i]);
	}
	return res;
}

template<typename T, typename U>
vector<T> operator*(vector<T> v, U s) {
	transform(v.begin(), v.end(), v.begin(), [&s](T t) { return t * s; });
	return v;
}

template<typename T>
vector<T> operator+(vector<T> v1, vector<T> v2) {
	if (v1.size() != v2.size()) {
		cout << "Size " << v1.size()
			 << " and " << v2.size()
			 << " are incompatible" << endl;
		exit(SIZE_ERROR);
	}
	vector<T> res;
	for (int i = 0; i < v1.size(); i++) {
		res.push_back(v1[i] + v2[i]);
	}
	return res;
}

double getNormalValue(double x, double mu, double sig);

#endif //UTILS_H

//
// Created by samuel on 07/10/18.
//

#include "utils.h"
#include <iostream>

double getNormalValue(double x, double mu, double sig) {
	return exp(-0.5 * pow((x - mu) / sig, 2.)) / (sig * sqrt(2. * M_PI));
}

vector<double> getNormalValues(int n, double mu, double sig) {
	vector<double> res((unsigned long) n);
	transform(res.begin(), res.end(), res.begin(),
			  [mu,sig](double d){ return getNormalValue(2. * double(rand()) / double(RAND_MAX) - 1., mu, sig); });
	return res;
}

vector<vector<double>> toDiag(vector<double> v) {
	vector<vector<double>> res(v.size(), vector<double>(v.size()));
	for (int i = 0; i < v.size(); i++) {
		res[i][i] = v[i];
	}
	return res;
}

double norm(vector<double> v) {
	double res = 0.;
	for_each(v.begin(), v.end(), [&res](double d){ res += d; });
	return res / double(v.size());
}


vector<double> multMV(vector<double> v, vector<vector<double>> m) {
	vector<double> res(v.size(), 0.);

	if (m.size() != v.size()) {
		cout << "Sizes incompatible : "
			 << m.size() << " "
			 << v.size() << endl;
		exit(SIZE_ERROR);
	}

	for (int i = 0; i < m.size(); i++) {
		res[i] = dot(v, m[i]);
	}

	return res;
}

double dot(vector<double> v1, vector<double> v2) {
	if (v1.size() != v2.size()) {
		cout << "Sizes incompatible : "
			 << v1.size() << " "
			 << v2.size() << endl;
		exit(SIZE_ERROR);
	}
	double res = 0;
	for (int i = 0; i < v1.size(); i++) {
		res += v1[i] * v2[i];
	}
	return res;
}


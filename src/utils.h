#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>

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

#endif //UTILS_H

#include "utils.h"

int main(int argc, char **argv) {
  vector<vector<float>> v1;
  vector<vector<float>> v2;

  for (int i = 0; i < 10; i++) {
    vector<float> tmp1;
    vector<float> tmp2;

    for (int j = 0; j < 10; j++) {
      tmp1.push_back(2.f);
      tmp2.push_back(i * 10 + j);
    }

    v1.push_back(tmp1);
    v2.push_back(tmp2);
  }

  vector<vector<float>> res = v1 * v2;

  for (int i = 0; i < res.size(); i++) {
    for (int j = 0; j < res[i].size(); j++)
      cout << res[i][j] << ", ";
    cout << endl;
  }


  return 0;
}

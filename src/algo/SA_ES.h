//
// Created by samuel on 07/10/18.
//

#ifndef OPTIMISATIONM2AIC_SA_ES_H
#define OPTIMISATIONM2AIC_SA_ES_H

#include "../coco.h"
#include "../utils/utils.h"

#define RATIO 4 / 5
#define SIZE 5

using namespace std;

class SA_ES {
private:
	int n;

	normal_distribution<double> normal_dist;
	uniform_real_distribution<double> uniform_dist;
	default_random_engine generator;

	int lambda;
	int mu;

	double tau;
	double tauI;

	const double *minValues;
	const double *maxValues;

	coco_problem_s *problem;

	individual parent;

	vector<double> makeVector(double min, double range);
	vector<double> makeXVector();

public:
	explicit SA_ES(coco_problem_s *p);

	void step();

	double getParentValue();
};


#endif //OPTIMISATIONM2AIC_SA_ES_H

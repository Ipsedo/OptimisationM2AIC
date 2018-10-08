//
// Created by samuel on 07/10/18.
//

#ifndef OPTIMISATIONM2AIC_SA_ES_H
#define OPTIMISATIONM2AIC_SA_ES_H

#include <vector>
#include "../coco.h"
#include "../utils/utils.h"

using namespace std;

class SA_ES {
private:
	int n;
	int d;

	normal_distribution<double> normal_dist;
	default_random_engine generator;

	int lambda;
	int mu;

	double tau;
	double tauI;

	coco_problem_s *problem;

	individual parent;

	vector<double> makeZerosVector();

public:
	explicit SA_ES(coco_problem_s *p);

	void step();
};


#endif //OPTIMISATIONM2AIC_SA_ES_H

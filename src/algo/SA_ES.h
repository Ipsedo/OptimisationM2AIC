//
// Created by samuel on 07/10/18.
//

#ifndef OPTIMISATIONM2AIC_SA_ES_H
#define OPTIMISATIONM2AIC_SA_ES_H

#include <vector>
#include "../coco.h"
#include "../utils.h"

using namespace std;

class SA_ES {
private:
	int n;

	int lambda;
	int mu;

	double tau;
	vector<double> taus;

	coco_problem_s *problem;

	individual parent;

	void step();

public:
	SA_ES(coco_problem_s *p, int dim);
};


#endif //OPTIMISATIONM2AIC_SA_ES_H

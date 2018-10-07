//
// Created by samuel on 07/10/18.
//

#include <algorithm>
#include "SA_ES.h"

#define SEED 123456

// tau ≈ 1 / sqrt(n) ? ajouter random ?
SA_ES::SA_ES(coco_problem_s *p, int dim)
		: problem(p), n(dim), lambda(5 * n), mu(lambda / 4), tau(1. / sqrt(n)),
		  taus(vector<double>((unsigned long) n)) {
	generate(taus.begin(), taus.end(), [this](){ return 1. / pow(n, 1. / 4.); });
	srand(SEED);
}

void SA_ES::step() {
	// Create childrens
	vector<individual> childens;
	for (int i = 0; i < lambda; i++) {
		individual c {vector<double>((unsigned long) n), vector<double>((unsigned long) n), 0.};
		double xi = tau * getNormalValue(double(rand()) / double(RAND_MAX), 0., 1.); // quelles bornes pour tirer un nb selon loi normale ?
		vector<double> xis((unsigned long) n);
		vector<double> z((unsigned long) n);

		//c.f_value = ?
		//coco_evaluate_function(problem, c.x.data(), ?)
		childens.push_back(c);
	}

	// Sort for select best
	sort(childens.begin(), childens.end(),
		 [](individual i1, individual i2){
			 if (i1.f_value == i2.f_value) return 0;
			 else if (i1.f_value > i2.f_value) return 1;
			 else return -1;});

	// Recombination
	parent.x = parent.x * 0.;
	parent.s = parent.s * 0.;
	double invMu = (1. / double(mu));
	for (int i = 0; i < mu; i++) {
		parent.x = parent.x + childens[i].x * invMu;
		parent.s = parent.s + childens[i].s * invMu;
	}
}

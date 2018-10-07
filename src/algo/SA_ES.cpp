//
// Created by samuel on 07/10/18.
//

#include <algorithm>
#include "SA_ES.h"

#define NOISE 1e-1

// tau ≈ 1 / sqrt(n) ? ajouter random ?
SA_ES::SA_ES(coco_problem_s *p)
		: problem(p), n(static_cast<int>(coco_problem_get_dimension(p))),
		  d(static_cast<int>(coco_problem_get_number_of_objectives(p))),
		  lambda(5 * n), mu(lambda / 4),
		  tau(1. / sqrt(n) + NOISE * double(rand()) / double(RAND_MAX)), taus(vector<double>((unsigned long) n)),
		  parent(individual{vector<double>((unsigned long) n, 0.), vector<double>((unsigned long) n, 0.), 0.}),
			distribution(0., 1.), generator() {
	generate(taus.begin(), taus.end(), [this](){ return 1. / pow(n, 1. / 4.) + NOISE * double(rand()) / double(RAND_MAX); });
}

void SA_ES::step() {
	// Create childrens
	vector<individual> childens;
	for (int i = 0; i < lambda; i++) {
		individual c {vector<double>((unsigned long) n), vector<double>((unsigned long) n), 0.};

		double xi = tau * distribution(generator);
		vector<double> xis = taus * getNormalValues(n, distribution, generator);
		vector<double> z = getNormalValues(n, distribution, generator);

		// mutation
		transform(xis.begin(), xis.end(), xis.begin(), [](double d) { return exp(d); });
		c.s = parent.s * xis * exp(xi);
		c.x = parent.x + c.s * z;

		coco_evaluate_function(problem, c.x.data(), &c.f_value);

		childens.push_back(c);
	}

	// Sort for select best
	sort(childens.begin(), childens.end(),
		 [](individual i1, individual i2){
			 if (i1.f_value > i2.f_value) return 1;
			 else if (i1.f_value < i2.f_value) return -1;
			 else return 0;});

	// Recombination
	for (int i = 0; i < n; i++) {
		parent.x[i] = 0.;
		parent.s[i] = 0.;
	}
	double invMu = (1. / double(mu));
	for (int i = 0; i < mu; i++) {
		parent.x = parent.x + childens[i].x * invMu;
		parent.s = parent.s + childens[i].s * invMu;
	}
}

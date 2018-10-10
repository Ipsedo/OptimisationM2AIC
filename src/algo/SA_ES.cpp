//
// Created by samuel on 07/10/18.
//

#include <algorithm>
#include "SA_ES.h"

SA_ES::SA_ES(coco_problem_s *p)
		: normal_dist(0., 1.), uniform_dist(0., 1.), generator(),
		  problem(p), n(static_cast<int>(coco_problem_get_dimension(p))),
		  d(static_cast<int>(coco_problem_get_number_of_objectives(p))),
		  lambda(100 * n), mu(lambda / 4),
		  tau(1. / sqrt(n)), tauI(1. / pow(n, 1. / 4.)),
		  parent(individual{makeVector(-1., 2.), makeVector(numeric_limits<double>::min(), 1.), 0.}) {
}

void SA_ES::step() {
	// Create childrens
	vector<individual> childens;
	for (int i = 0; i < lambda; i++) {
		individual c;

		double xi = tau * normal_dist(generator);
		vector<double> xis = getNormalValues(n, normal_dist, generator) * tauI;
		vector<double> z = getNormalValues(n, normal_dist, generator);

		// mutation
		transform(xis.begin(), xis.end(), xis.begin(), [](double d) { return exp(d); });
		c.s = parent.s * xis * exp(xi);
		c.x = parent.x + c.s * z;

		coco_evaluate_function(problem, c.x.data(), &c.f_value);

		childens.push_back(c);

		xis.clear();
		z.clear();
	}

	// Sort for select best
	sort(childens.begin(), childens.end(), [](individual i1, individual i2){ return i1.f_value > i2.f_value; });

	// Recombination
	for (int i = 0; i < n; i++) {
		parent.x[i] = 0.;
		parent.s[i] = 0.;
	}
	double invMu = 1. / double(mu);
	for (int i = 0; i < mu; i++) {
		parent.x = parent.x + childens[i].x * invMu;
		parent.s = parent.s + childens[i].s * invMu;
	}
	for (individual i : childens) {
		i.x.clear();
		i.s.clear();
	}
	childens.clear();
}

vector<double> SA_ES::makeVector(double min, double range) {
	return vector<double>((unsigned long) n, range * uniform_dist(generator) - min);
}

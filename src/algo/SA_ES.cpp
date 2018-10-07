//
// Created by samuel on 07/10/18.
//

#include <algorithm>
#include "SA_ES.h"

#define SEED 123456

// tau ≈ 1 / sqrt(n) ? ajouter random ?
SA_ES::SA_ES(coco_problem_s *p)
		: problem(p), n(static_cast<int>(coco_problem_get_dimension(p))),
		  d(static_cast<int>(coco_problem_get_number_of_objectives(p))),
		  /*nbConstraint(static_cast<int>(coco_problem_get_number_of_constraints(p))),*/
		  lambda(5 * n), mu(lambda / 4),
		  tau(1. / sqrt(n)), taus(vector<double>((unsigned long) n)),
		  parent(individual{vector<double>((unsigned long) n), vector<double>((unsigned long) n), 0.}){
	generate(taus.begin(), taus.end(), [this](){ return 1. / pow(n, 1. / 4.); });
	srand(SEED);
}

void SA_ES::step() {
	// Create childrens
	vector<individual> childens;
	for (int i = 0; i < lambda; i++) {
		individual c {vector<double>((unsigned long) n), vector<double>((unsigned long) n), 0.};

		double xi = tau *
				getNormalValue(2. * double(rand()) / double(RAND_MAX) - 1.,
										 0., 1.); // quelles bornes pour tirer un nb selon loi normale ?
		vector<double> xis = taus * getNormalValues(n, 0., 1.);
		vector<double> z = getNormalValues(n, 0., 1.);

		// mutation
		transform(xis.begin(), xis.end(), xis.begin(), [](double d) { return exp(d); });
		c.s = parent.s * xis * exp(xi);
		c.x = parent.x + c.s * z;

		//c.f_value = ?
		vector<double> y((unsigned long) d);
		coco_evaluate_function(problem, c.x.data(), y.data());
		c.f_value = norm(y);

		/*if (nbConstraint > 0) {
			vector<double> cons((unsigned long) nbConstraint);
			coco_evaluate_constraint(problem, c.x.data(), cons.data());
			cons.clear();
		}*/
		y.clear();

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

//
// Created by samuel on 07/10/18.
//

#include "utils.h"

double getNormalValue(double x, double mu, double sig) {
	return exp(-0.5 * pow((x - mu) / sig, 2.)) / (sig * sqrt(2. * M_PI));
}
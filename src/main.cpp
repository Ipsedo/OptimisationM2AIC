/**
 * An example of benchmarking random search on a COCO suite. A grid search optimizer is also
 * implemented and can be used instead of random search.
 *
 * Set the global parameter BUDGET_MULTIPLIER to suit your needs.
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "coco.h"
#include "algo/SA_ES.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * The maximal budget for evaluations done by an optimization algorithm equals dimension * BUDGET_MULTIPLIER.
 * Increase the budget multiplier value gradually to see how it affects the runtime.
 */
static const unsigned int BUDGET_MULTIPLIER = 1000;

/**
 * The maximal number of independent restarts allowed for an algorithm that restarts itself.
 */
static const long INDEPENDENT_RESTARTS = 1e5;

/**
 * The random seed. Change if needed.
 */
static const uint32_t RANDOM_SEED = 0xdeadbeef;

/**
 * A function type for evaluation functions, where the first argument is the vector to be evaluated and the
 * second argument the vector to which the evaluation result is stored.
 */
typedef void (*evaluate_function_t)(const double *x, double *y);

/**
 * A pointer to the problem to be optimized (needed in order to simplify the interface between the optimization
 * algorithm and the COCO platform).
 */
static coco_problem_t *PROBLEM;


/* Declarations of all functions implemented in this file (so that their order is not important): */
void example_experiment(const char *suite_name,
						const char *observer_name);

/* Structure and functions needed for timing the experiment */
typedef struct {
	size_t number_of_dimensions;
	size_t current_idx;
	char **output;
	size_t previous_dimension;
	size_t cumulative_evaluations;
	time_t start_time;
	time_t overall_start_time;
} timing_data_t;

static timing_data_t *timing_data_initialize(coco_suite_t *suite);

static void timing_data_time_problem(timing_data_t *timing_data, coco_problem_t *problem);

static void timing_data_finalize(timing_data_t *timing_data);

#define SEED 54518941

/**
 * The main method initializes the random number generator and calls the example experiment on the
 * bi-objective suite.
 */
int main(void) {

	srand(SEED);

	/* Change the log level to "warning" to get less output */
	coco_set_log_level("info");

	printf("Running the example experiment... (might take time, be patient)\n");
	fflush(stdout);

	/**
	 * Start the actual experiments on a test suite and use a matching logger, for
	 * example one of the following:
	 *   bbob                 24 unconstrained noiseless single-objective functions
	 *   bbob-biobj           55 unconstrained noiseless bi-objective functions
	 *   bbob-biobj-ext       92 unconstrained noiseless bi-objective functions
	 *   [bbob-constrained*   48 constrained noiseless single-objective functions]
	 *   [bbob-largescale*    24 unconstrained noiseless single-objective functions in large dimension]
	 *
	 * Suites with a star are partly implemented but not yet fully supported.
	 *
	 * Adapt to your need. Note that the experiment is run according
	 * to the settings, defined in example_experiment(...) below.
	 */
	example_experiment("bbob", "bbob");

	printf("Done!\n");
	fflush(stdout);

	return 0;
}

/**
 * A simple example of benchmarking random search on a given suite with default instances
 * that can serve also as a timing experiment.
 *
 * @param suite_name Name of the suite (e.g. "bbob" or "bbob-biobj").
 * @param observer_name Name of the observer matching with the chosen suite (e.g. "bbob-biobj"
 * when using the "bbob-biobj-ext" suite).
 * @param random_generator The random number generator.
 */
void example_experiment(const char *suite_name,
						const char *observer_name) {

	size_t run;
	coco_suite_t *suite;
	coco_observer_t *observer;
	timing_data_t *timing_data;

	/* Set some options for the observer. See documentation for other options. */
	char *observer_options =
			coco_strdupf("result_folder: SA_ES_on_%s "
								 "algorithm_name: SA_ES "
								 "algorithm_info: \"A simple random search algorithm\"", suite_name);

	/* Initialize the suite and observer.
	 *
	 * For more details on how to change the default options, see
	 * http://numbbo.github.io/coco-doc/C/#suite-parameters and
	 * http://numbbo.github.io/coco-doc/C/#observer-parameters. */
	suite = coco_suite(suite_name, "", "dimensions: 20");
	observer = coco_observer(observer_name, observer_options);
	coco_free_memory(observer_options);

	/* Initialize timing */
	timing_data = timing_data_initialize(suite);

	/* Iterate over all problems in the suite */
	while ((PROBLEM = coco_suite_get_next_problem(suite, observer)) != NULL) {

		size_t dimension = coco_problem_get_dimension(PROBLEM);

		/* Run the algorithm at least once */
		for (run = 1; run <= 1 + INDEPENDENT_RESTARTS; run++) {

			long evaluations_done = (long) (coco_problem_get_evaluations(PROBLEM) +
											coco_problem_get_evaluations_constraints(PROBLEM));
			long evaluations_remaining = (long) (dimension * BUDGET_MULTIPLIER) - evaluations_done;

			/* Break the loop if the target was hit or there are no more remaining evaluations */
			if ((coco_problem_final_target_hit(PROBLEM) &&
				 coco_problem_get_number_of_constraints(PROBLEM) == 0)
				|| (evaluations_remaining <= 0))
				break;

			/** ALGO SA_ES */
			SA_ES sa_es(PROBLEM);
			for (int i = 0; i < 50; i++) {
				sa_es.step();
			}
			//cout << endl;
			//cout << sa_es.getParentValue() << endl;

			/* Break the loop if the algorithm performed no evaluations or an unexpected thing happened */
			if (coco_problem_get_evaluations(PROBLEM) == evaluations_done) {
				printf("WARNING: Budget has not been exhausted (%lu/%lu evaluations done)!\n",
					   (unsigned long) evaluations_done, (unsigned long) dimension * BUDGET_MULTIPLIER);
				break;
			} else if (coco_problem_get_evaluations(PROBLEM) < evaluations_done)
				coco_error("Something unexpected happened - function evaluations were decreased!");
		}

		/* Keep track of time */
		timing_data_time_problem(timing_data, PROBLEM);
	}

	/* Output and finalize the timing data */
	timing_data_finalize(timing_data);

	coco_observer_free(observer);
	coco_suite_free(suite);

}

/**
 * Allocates memory for the timing_data_t object and initializes it.
 */
static timing_data_t *timing_data_initialize(coco_suite_t *suite) {

	timing_data_t *timing_data = (timing_data_t *) coco_allocate_memory(sizeof(*timing_data));
	size_t function_idx, dimension_idx, instance_idx, i;

	/* Find out the number of all dimensions */
	coco_suite_decode_problem_index(suite, coco_suite_get_number_of_problems(suite) - 1, &function_idx,
									&dimension_idx, &instance_idx);
	timing_data->number_of_dimensions = dimension_idx + 1;
	timing_data->current_idx = 0;
	timing_data->output = (char **) coco_allocate_memory(timing_data->number_of_dimensions * sizeof(char *));
	for (i = 0; i < timing_data->number_of_dimensions; i++) {
		timing_data->output[i] = NULL;
	}
	timing_data->previous_dimension = 0;
	timing_data->cumulative_evaluations = 0;
	time(&timing_data->start_time);
	time(&timing_data->overall_start_time);

	return timing_data;
}

/**
 * Keeps track of the total number of evaluations and elapsed time. Produces an output string when the
 * current problem is of a different dimension than the previous one or when NULL.
 */
static void timing_data_time_problem(timing_data_t *timing_data, coco_problem_t *problem) {

	double elapsed_seconds = 0;

	if ((problem == NULL) || (timing_data->previous_dimension != coco_problem_get_dimension(problem))) {

		/* Output existing timing information */
		if (timing_data->cumulative_evaluations > 0) {
			time_t now;
			time(&now);
			elapsed_seconds = difftime(now, timing_data->start_time) / (double) timing_data->cumulative_evaluations;
			timing_data->output[timing_data->current_idx++] = coco_strdupf("d=%lu done in %.2e seconds/evaluation\n",
																		   timing_data->previous_dimension,
																		   elapsed_seconds);
		}

		if (problem != NULL) {
			/* Re-initialize the timing_data */
			timing_data->previous_dimension = coco_problem_get_dimension(problem);
			timing_data->cumulative_evaluations = coco_problem_get_evaluations(problem);
			time(&timing_data->start_time);
		}

	} else {
		timing_data->cumulative_evaluations += coco_problem_get_evaluations(problem);
	}
}

/**
 * Outputs and finalizes the given timing data.
 */
static void timing_data_finalize(timing_data_t *timing_data) {

	/* Record the last problem */
	timing_data_time_problem(timing_data, NULL);

	if (timing_data) {
		size_t i;
		double elapsed_seconds;
		time_t now;
		int hours, minutes, seconds;

		time(&now);
		elapsed_seconds = difftime(now, timing_data->overall_start_time);

		printf("\n");
		for (i = 0; i < timing_data->number_of_dimensions; i++) {
			if (timing_data->output[i]) {
				printf("%s", timing_data->output[i]);
				coco_free_memory(timing_data->output[i]);
			}
		}
		hours = (int) elapsed_seconds / 3600;
		minutes = ((int) elapsed_seconds % 3600) / 60;
		seconds = (int) elapsed_seconds - (hours * 3600) - (minutes * 60);
		printf("Total elapsed time: %dh%02dm%02ds\n", hours, minutes, seconds);

		coco_free_memory(timing_data->output);
		coco_free_memory(timing_data);
	}
}

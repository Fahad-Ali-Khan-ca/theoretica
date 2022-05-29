
///
/// @file test.h Routines to test the precision of the library
///

#ifndef THEORETICA_TEST_H
#define THEORETICA_TEST_H

#include "../src/theoretica.h"
#include <iostream>

using namespace th;

// Absolute difference to tolerate

#ifndef THEORETICA_TOLERANCE
#define THEORETICA_TOLERANCE 0.00000001
#endif

// Default tolerance
// 10^-8
constexpr real TOLERANCE = THEORETICA_TOLERANCE;

// Function name holder
std::string func_name = "";

// Total number of errors (module-wise)
unsigned int total_errors = 0;

// Number of test_tolr test runs
unsigned int tolr_test_runs = 0;

// Number of errors on current function
unsigned int curr_errors = 0;

// Cumulative error on current function
long double cum_err = 0;

// Cumulative error on current function
long double cum_sqr_err = 0;

// Maximum error on current function
long double max_err = 0;

// Sum of relative errors
long double rel_err_sum = 0;

// Number of added relative errors
unsigned int rel_err_n = 0;

// Start testing a specific function
void test_start(const std::string& f) {

	func_name = f;
	std::cout << "Testing " << func_name << " ...\n" << std::endl;

	curr_errors = 0;
	tolr_test_runs = 0;
	cum_err = 0;
	cum_sqr_err = 0;
	max_err = 0;
	rel_err_sum = 0;
	rel_err_n = 0;
}


// End testing on the current function and print out information
// about the test runs
void test_end() {

	std::cout << "\nFinished testing " << func_name << std::endl;

	if(curr_errors == 0) {
		std::cout << "All tests passed successfully" << std::endl;
	} else {
		std::cout << curr_errors << " tests failed (" <<
		(curr_errors / (real) tolr_test_runs * 100) << "%)" << std::endl;
	}

	std::cout << "Mean Error: " << (cum_err / (real) tolr_test_runs) << std::endl;
	std::cout << "RMS Error: " << th::sqrt(cum_sqr_err / (real) tolr_test_runs) << std::endl;
	std::cout << "Maximum Error: " << max_err << std::endl;
	std::cout << "Mean Relative Error: " << rel_err_sum / (real) tolr_test_runs * 100 << " %\n\n" << std::endl;
	func_name = "";
}


// Check whether the given values differ only by a tolerance value or less
template<typename T>
bool good_enough(T a, T b, real tolerance = TOLERANCE) {
	return th::abs(b - a) < tolerance;
}


// Compare a function to an expected value
template<typename T1, typename T2>
void test_equal(T1 evaluated, T1 expected, T2 input, bool silent = false) {

	if(!(evaluated == expected)) {

		if(!silent) {
			std::cout << "\tTest failed" << std::endl;
		}
		
		total_errors++;
		curr_errors++;

	} else if(!silent) {
		std::cout << "\tTest passed" << std::endl;
	}
}


// Test a real function on an interval
void test_equal_interval(real_function f, real_function f_exp, real a, real b,
	real tolerance = TOLERANCE, unsigned int steps = 1000) {

	std::cout << "\tTesting on interval [" << a << ", " << b << "] with " << steps << " steps" << std::endl;

	real dx = (b - a) / (real) steps;

	for (int i = 0; i <= steps; ++i) {

		real x = a + i * dx;

		test_equal(f(x), f_exp(x), x, true);
	}
}


// Compare a function to an expected value
template<typename T1, typename T2>
void test_tol(T1 evaluated, T1 expected, T2 input, real tolerance = TOLERANCE, bool silent = false) {

	real diff = th::abs(evaluated - expected);
	cum_err += diff;
	cum_sqr_err += th::square(diff);
	max_err = th::max(diff, max_err);
	tolr_test_runs++;

	// Absolute difference
	real curr_err = diff;

	// If the expected value is not too small,
	// use the relative error
	if(th::abs(expected) > tolerance) {
		curr_err /= th::abs(expected);
		rel_err_sum += curr_err;
		rel_err_n++;
	}

	if(!(curr_err < tolerance)) {

		if(!silent) {
			std::cout << "\tTest failed on " << func_name << ":" << std::endl;
			std::cout << "\t\tExpected: " << expected << std::endl;
			std::cout << "\t\tEvaluated: " << evaluated << std::endl;
			std::cout << "\t\tInput: " << input << std::endl;
			std::cout << "\t\tDiff: " << diff << std::endl;
		}
		
		total_errors++;
		curr_errors++;

	} else if(!silent) {
		std::cout << "\tTest passed with diff: " << diff << std::endl;
	}
}


void test_tolr(real evaluated, real expected, real input, real tolerance = TOLERANCE, bool silent = false) {
	test_tol<real, real>(evaluated, expected, input, tolerance, silent);
}



// Test a real function on an interval
void test_tolr_interval(real_function f, real_function f_exp, real a, real b,
	real tolerance = TOLERANCE, unsigned int steps = 1000) {

	std::cout << "\tTesting on interval [" << a << ", " << b << "] with " << steps << " steps" << std::endl;

	real dx = (b - a) / (real) steps;

	real cum_err_ = cum_err;
	real cum_sqr_err_ = cum_sqr_err;
	real max_err_ = max_err;

	cum_err = 0;
	cum_sqr_err = 0;
	max_err = 0;

	for (int i = 0; i <= steps; ++i) {

		real x = a + i * dx;

		test_tolr(f(x), f_exp(x), x, tolerance, true);
	}

	std::cout << "\tMean Error on Interval: " << (cum_err / th::abs(b - a) / (real) steps) << std::endl;
	std::cout << "\tRMS Error on Interval: " << th::sqrt(cum_sqr_err / th::abs(b - a) / (real) steps) << std::endl;
	std::cout << "\tMaximum Error on Interval: " << max_err << "\n" << std::endl;

	real cum_err = cum_err_;
	real cum_sqr_err = cum_sqr_err_;
	real max_err = max_err_;
}


#endif

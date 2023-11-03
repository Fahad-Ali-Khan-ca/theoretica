
#include "theoretica.h"
#include <cmath>
#include <ctime>
#include "chebyshev/prec.h"

using namespace chebyshev;
using namespace theoretica;


int main(int argc, char const *argv[]) {

	prec::state.outputFolder = "test/";
	prec::setup("statistics");

		PRNG g = PRNG::xoshiro(time(nullptr));

		// Distributions

		for (int i = 1; i <= 10; ++i) {

			real alpha = i;
			real beta = 1;

			// Check mean of Gamma distribution
			prec::equals(
				"gamma_dist (1)",
				integral_gauss(
					[alpha, beta](real x) {
						return x * distribution::gamma_dist(x, alpha, beta);
					},
					tables::laguerre_roots_16, tables::laguerre_weights_16,
					16, [](real x) { return th::exp(x); }
				),
				(alpha / beta)
			);
		}


		// P-value

		for (int i = 0; i < 10; ++i) {

			unsigned int chi = g() % 300 + 1;
			unsigned int ndf = g() % 300 + 1;

			std::stringstream str;
			str << "pvalue_...(" << chi << "," << ndf << ") < 1";

			prec::equals(str.str(),
				pvalue_chi_squared(chi, ndf) < 1, 1);
		}

		for (int i = 0; i < 10; ++i) {

			unsigned int ndf = g() % 300 + 1;

			std::stringstream str;
			str << "pvalue_...(0," << ndf << ")";

			prec::equals(str.str(),
				pvalue_chi_squared(0, ndf), 1, 1E-6);
		}

	prec::terminate();
}

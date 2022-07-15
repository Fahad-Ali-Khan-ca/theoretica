
///
/// @file derivation.h Derivative approximation
///

#ifndef DERIVATION_THEORETICA_H
#define DERIVATION_THEORETICA_H

#include "../core/function.h"
#include "../polynomial/polynomial.h"


namespace theoretica {


	// Derivate a polynomial
	template<typename T>
	polynomial<T> deriv_polynomial(polynomial<T> p) {

		polynomial<T> Dp;

		for (unsigned int i = 1; i < p.size(); ++i)
			Dp.coeff.push_back(p[i] * i);

		return Dp;
	}


	// Central derivative approximation
	real deriv_central(real_function f, real x, real dx = 0.00000001) {
		return (f(x + dx) - f(x - dx)) / (2.0 * dx);
	}


	// Forward derivative approximation
	real deriv_forward(real_function f, real x, real dx = 0.00000001) {
		return (f(x + dx) - f(x)) / dx;
	}

	// Backward derivative approximation
	real deriv_backward(real_function f, real x, real dx = 0.00000001) {
		return (f(x) - f(x - dx)) / dx;
	}

}


#endif

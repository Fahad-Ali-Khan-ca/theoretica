#ifndef UROBORO_MULTIDUAL_H
#define UROBORO_MULTIDUAL_H


///
/// @file multidual.h Multidual numbers
///

#ifndef UROBORO_NO_PRINT
#include <sstream>
#include <ostream>
#endif

#include "../algebra/vec.h"


namespace uroboro {

	/// 
	/// @class multidual
	/// Multidual number algebra for functions
	/// of the form \f$f: \mathbb{R}^n \rightarrow \mathbb{R}\f$
	///
	template<unsigned int N>
	class multidual {
		
		public:
			real a;
			vec<N> v;


			/// Construct a multidual number
			/// as \f$(0 + \vec 0)\f$
			multidual() {
				a = 0;
				v = vec<N>(0);
			}


			/// Construct a multidual number from
			/// a real number and an N dimensional vector
			multidual(real r, vec<N> u) {
				a = r;
				v = u;
			}


			/// Construct a multidual number from
			/// a real number
			multidual(real r) {
				a = r;
				v = vec<N>(0);
			}


			/// Construct a multidual number from
			/// a (1 + N) dimensional vector
			multidual(const vec<1 + N>& data) {
				
				a = data.get(0);

				for (int i = 1; i < N + 1; ++i) {
					v.at(i - 1) = data.get(i);
				}
			}

			~multidual() = default;


			/// Initialize a multidual number from a real number
			inline multidual& operator=(real x) {
				a = x;
				v = vec<N>();
				return *this;
			}


			/// Return real part
			inline real Re() const {
				return a;
			}


			/// Return multidual part
			inline vec<N> Dual() const {
				return v;
			}


			/// Get the multidual conjugate
			inline multidual conjugate() const {
				return multidual(a, -v);
			}


			/// Get the inverse of a multidual number
			inline multidual inverse() const {

				if(a == 0) {
					UMATH_ERROR("multidual::inverse", 0, DIV_BY_ZERO);
					return multidual(nan(), vec<N>(nan()));
				}

				return multidual(1.0 / a, v * (-1 / (a * a)));
			}


			/// Identity (for consistency)
			inline multidual operator+() const {
				return multidual(a, v);
			}


			/// Sum two multidual numbers
			inline multidual operator+(const multidual& other) const {
				return multidual(a + other.a, v + other.v);
			}


			/// Sum a real number to a multidual number
			inline multidual operator+(real r) const {
				return multidual(a + r, v);
			}


			/// Get the opposite of a multidual number
			inline multidual operator-() const {
				return multidual(-a, -v);
			}


			/// Subtract two multidual numbers
			inline multidual operator-(const multidual& other) const {
				return multidual(a - other.a, v - other.v);
			}


			/// Subtract a real number from a multidual number
			inline multidual operator-(real r) const {
				return multidual(a - r, v);
			}


			/// Multiply two multidual numbers
			inline multidual operator*(const multidual& other) const {
				return multidual(a * other.a, other.v * a + v * other.a);
			}


			/// Multiply a multidual number by a real number
			inline multidual operator*(real r) const {
				return multidual(a * r, v * r);
			}


			/// Dual division
			inline multidual operator/(const multidual& other) const {

				if(a == 0) {
					UMATH_ERROR("multidual::operator/", 0, DIV_BY_ZERO);
					return multidual(nan(), vec<N>(nan()));
				}

				return multidual(a / other.a,
					(v * other.a - other.v * a) / (other.a * other.a));
			}

			/// Divide a multidual number by a real number
			inline multidual operator/(real r) const {
				return multidual(a / r, v / r);
			}


			/// Sum a real number to this multidual number
			inline multidual& operator+=(real r) {

				a += r;
				return *this;
			}

			/// Subtract a real number from this one
			inline multidual& operator-=(const multidual& other) {

				a -= other.a;
				v -= other.v;
				return *this;
			}

			/// Subtract a real number from this multidual number
			inline multidual& operator-=(real r) {

				a -= r;
				return *this;
			}

			/// Multiply this multidual number by another one
			inline multidual& operator*=(const multidual& other) {

				a = (a * other.a);
				v = (other.v * a) + (v * other.a);
				return *this;
			}

			/// Multiply this multidual number by a real number
			inline multidual& operator*=(real r) {

				a *= r;
				v *= r;
				return *this;
			}

			/// Divide this multidual number by another one
			inline multidual& operator/=(const multidual& other) {

				a = (a / other.a);
				v = (v * other.a - other.v * a) / (other.a * other.a);
				return *this;
			}

			/// Divide a multidual number by a real number
			inline multidual& operator/=(real r) {

				if(r == 0) {
					UMATH_ERROR("multidual::operator/=", 0, DIV_BY_ZERO);
					a = nan();
					v = vec<N>(nan());
					return *this;
				}

				a /= r;
				v /= r;

				return *this;
			}


			/// Check whether two multidual numbers have the same
			/// real and multidual parts
			inline bool operator==(const multidual& other) {
				return (a == other.a) && (v == other.v);
			}


			/// Construct an N-dimensional vector of multidual numbers
			/// to be passed as argument to a multidual function
			/// @param x A vector of real numbers containing the variables to pass
			inline static vec<N, multidual<N>> pack_function_arg(const vec<N, real>& x) {

				vec<N, multidual<N>> arg;
				for (int i = 0; i < N; ++i)
					arg.at(i) = multidual<N>(x.get(i), vec<N>::euclidean_base(i));

				return arg;
			}


			// Friend operators to enable equations of the form
			// (real) op. (multidual)

			inline friend multidual operator+(real a, const multidual& d) {
				return d + a;
			}

			inline friend multidual operator-(real a, const multidual& d) {
				return -d + a;
			}

			inline friend multidual operator*(real a, const multidual& d) {
				return d * a;
			}

			inline friend multidual operator/(real a, const multidual& d) {
				return multidual(a) / d;
			}


#ifndef UROBORO_NO_PRINT

			/// Convert the multidual number to string representation
			/// @param epsilon The character to use to represent epsilon
			inline std::string to_string(std::string epsilon = "e") const {

				std::stringstream res;
				res << a << " + " << v << epsilon;

				return res.str();
			}


			/// Stream the multidual number in string representation
			/// to an output stream (std::ostream)
			inline friend std::ostream& operator<<(std::ostream& out, const multidual& obj) {
				return out << obj.to_string();
			}

#endif
		
	};

}

#endif

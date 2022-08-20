default_target: all
.PHONY: all example test_real_analysis test_algebra automatic_differentiation hamiltonian_simulation \
		error_propagation statistics sampling_distributions multivariate_minimization benchmark clean

all: example test examples

CXXFLAGS = -std=c++14 -I./src/ -Wall

example:
	@echo Compiling main example program...
	@g++ examples/example.cpp ${CXXFLAGS} -o ./example


# Tests

test_algebra:
ifndef windows_build
	@echo Compiling linear algebra test cases...
	@g++ test/test_algebra.cpp ${CXXFLAGS} -I./test/ -o test/test_algebra
	@./test/test_algebra
endif

test_real_analysis:
ifndef windows_build
	@echo Compiling real analysis test cases...
	@g++ test/test_real_analysis.cpp ${CXXFLAGS} -I./test/ -o test/test_real_analysis
	@./test/test_real_analysis
endif

test: test_real_analysis test_algebra


# Example programs

automatic_differentiation:
	@echo Compiling \"Automatic differentiation\" example...
	@g++ examples/automatic_differentiation.cpp ${CXXFLAGS} -o ./automatic_differentiation

hamiltonian_simulation:
	@echo Compiling \"Hamiltonian simulation\" example...
	@g++ examples/hamiltonian_simulation.cpp ${CXXFLAGS} -o ./hamiltonian_simulation

error_propagation:
	@echo Compiling \"Error propagation\" example...
	@g++ examples/error_propagation.cpp ${CXXFLAGS} -o ./error_propagation

statistics:
	@echo Compiling \"Statistics\" example...
	@g++ examples/statistics.cpp ${CXXFLAGS} -o ./statistics

sampling_distributions:
	@echo Compiling \"Sampling distributions\" example...
	@g++ examples/sampling_distributions.cpp ${CXXFLAGS} -o ./sampling_distributions

montecarlo_comparison:
	@echo Compiling \"Montecarlo comparison\" example...
	@g++ examples/montecarlo_comparison.cpp ${CXXFLAGS} -o ./montecarlo_comparison

multivariate_minimization:
	@echo Compiling \"Multivariate minimization\" example...
	@g++ examples/multivariate_minimization.cpp ${CXXFLAGS} -o ./multivariate_minimization

examples: example automatic_differentiation hamiltonian_simulation error_propagation \
		  statistics sampling_distributions montecarlo_comparison multivariate_minimization


# Benchmarks

benchmark_real_analysis:
	@echo Compiling real analysis benchmark...
	@g++ benchmark/benchmark_real_analysis.cpp ${CXXFLAGS} -I./test/ -O0 -o benchmark/benchmark_real_analysis
	@./benchmark/benchmark_real_analysis

benchmark_algebra:
	@echo Compiling algebra benchmark...
	@g++ benchmark/benchmark_algebra.cpp ${CXXFLAGS} -I./test/ -O0 -o benchmark/benchmark_algebra
	@./benchmark/benchmark_algebra

benchmark: benchmark_real_analysis benchmark_algebra

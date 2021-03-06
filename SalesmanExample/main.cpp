////////////////////////////////////////////////////////////
//
// Copyright (c) 2018 Jan Filipowicz, Filip Turobos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////

#include <charconv>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <gsl/gsl_util>
#include <genetics.h>
#include "path_evaluator.h"
#include "path_merger.h"
#include "path_mutator.h"
#include "permutation.h"
#include "permutation_generator.h"

int main() {
	std::ios::sync_with_stdio(false);
	std::ifstream in("matrix.txt");
	std::size_t n;
	in >> n;
	std::vector<std::vector<long long>> matrix(n, std::vector<long long>(n));
	for (auto&& row : matrix) {
		for (auto&& cell : row) {
			in >> cell;
		}
	}
	std::mt19937_64 rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	using algorithm_type = genetic_algorithm<permutation, long long>;
	algorithm_type::context_type context;
	context.initial_population_size = 1000;
	context.breeding_population_size = 100;
	context.max_iterations = 100;
	context.generator = permutation_generator(n, rand);
	context.evaluator = path_evaluator(matrix);
	context.selector = elitist_selection<std::greater<>>();
	// Also try:
	// context.selector = roulette_wheel_selection(rand, [](long long x) { return std::exp(-x / 200.0); });
	context.breeder = mutating_breeder(path_merger(rand),
		chain_mutation {
			mutate_with_probability(rand, 0.2, path_node_swapper(rand)),
			mutate_with_probability(rand, 0.1, path_node_relocator(rand)),
		}
	);
	context.comparator = std::greater<>();
	std::ifstream in_pos("positions.txt");
	std::vector<std::pair<double, double>> positions(n);
	for (auto&& [x, y] : positions) {
		in_pos >> x >> y;
	}
	algorithm_type algorithm(context);
#ifdef LOGGING
	std::ofstream out_log("salesman.log");
	default_logger logger(out_log);
#endif
	for (int i = 0; i < 10; i++) {
		const auto result = algorithm(
#ifdef LOGGING
			logger
#endif
		);
		std::cout << "Best path found has length " << result.rating() << ":\n" << result.value() << std::endl;
		char filename[] = "cities_0.log";
		std::to_chars(&filename[7], &filename[8], i);
		std::ofstream out_pos(filename);
		for (const auto& index : result.value()) {
			const auto& [x, y] = gsl::at(positions, index);
			out_pos << x << ' ' << y << '\n';
		}
		const auto& [x, y] = gsl::at(positions, result.value().front());
		out_pos << x << ' ' << y << '\n';
	}
	return 0;
}

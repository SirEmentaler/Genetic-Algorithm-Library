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

#ifndef GENETIC_ALGORITHM_LIBRARY_MUTATE_WITH_PROBABILITY_H
#define GENETIC_ALGORITHM_LIBRARY_MUTATE_WITH_PROBABILITY_H

#include <random>

template<class UniformRandomBitGenerator, class Mutator>
class mutate_with_probability {
public:
	mutate_with_probability(UniformRandomBitGenerator& g, double probability, const Mutator& mutator);
	template<class Specimen>
	void operator()(Specimen& specimen);
private:
	UniformRandomBitGenerator& rand;
	std::bernoulli_distribution distribution;
	Mutator mutator;
};

template<class UniformRandomBitGenerator, class Mutator>
inline mutate_with_probability<UniformRandomBitGenerator, Mutator>::mutate_with_probability(UniformRandomBitGenerator& g, double probability, const Mutator& mutator)
	: rand(g), distribution(probability), mutator(mutator) {}

template<class UniformRandomBitGenerator, class Mutator>
template<class Specimen>
inline void mutate_with_probability<UniformRandomBitGenerator, Mutator>::operator()(Specimen& specimen) {
	if (distribution(rand))
		mutator(specimen);
}

#endif

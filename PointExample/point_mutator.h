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

#ifndef POINT_EXAMPLE_POINT_MUTATOR_H
#define POINT_EXAMPLE_POINT_MUTATOR_H

#include <random>
#include "point.h"

template<class UniformRandomBitGenerator>
class point_mutator {
public:
	point_mutator(double sigma, UniformRandomBitGenerator& g);
	void operator()(point& p);
private:
	std::normal_distribution<> distribution;
	UniformRandomBitGenerator& rand;
};

template<class UniformRandomBitGenerator>
inline point_mutator<UniformRandomBitGenerator>::point_mutator(double sigma, UniformRandomBitGenerator& g)
	: distribution(0.0, sigma), rand(g) {}

template<class UniformRandomBitGenerator>
inline void point_mutator<UniformRandomBitGenerator>::operator()(point& p) {
	p.x += distribution(rand);
	p.y += distribution(rand);
};

#endif

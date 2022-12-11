#pragma once

#include <iostream>
#include "Chromosome.h"
#include <initializer_list>
#include <vector>

namespace GASelection
{
	std::vector<Chromosome> RankChroms(std::vector<Chromosome>* chroms);
	std::vector<Chromosome> RankedSelection(std::vector<Chromosome>* gen, int selectNum, int parentNum);
	Chromosome onepointCrossOver(Chromosome parent1, Chromosome parent2);
	std::vector<Chromosome> LinearRankedSelection(std::vector<Chromosome>* gen, unsigned int seed, std::initializer_list<int> bandDists);
	std::vector<Chromosome> ElitistSelection(std::vector<Chromosome>* gen, unsigned int seed, std::vector<Chromosome> indivs, std::initializer_list<int> bandDists);
	std::vector<Chromosome> TournamentSelection(std::vector<Chromosome>* gen, unsigned int seed, int matingPoolSize, int tournySize);

	std::vector<Chromosome> StochasticSamplingSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum);
	std::vector<Chromosome> RouletteSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum);
}


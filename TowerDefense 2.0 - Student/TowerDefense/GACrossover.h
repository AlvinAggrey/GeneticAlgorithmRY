#pragma once
#include "Chromosome.h"
//chromsome and completed testing?
typedef std::pair<Chromosome, bool> individual;

enum class CrossoverMethod
{
	None,
	Onepoint,
};
class GACrossover
{
	std::random_device m_rd;

public:
	GACrossover();
	GACrossover(const GACrossover& other);
	GACrossover operator=(const GACrossover& other);

	std::mt19937 m_gen;

	//std::random_device rd;
	Chromosome Onepoint(Chromosome parent1, Chromosome parent2);
};


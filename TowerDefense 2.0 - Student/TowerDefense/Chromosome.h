#pragma once
#include <vector>
#include <random>

struct Gene
{
	//x and y
	int m_position[2] = {0,0};
	int m_towerType = 0;
};

class Chromosome
{
public:
	//random_device is exspensive so pass it in
	Chromosome(unsigned int rd);
	std::vector<Gene>& Genes();

private:
	const int m_chromsize = 15;
	std::vector<Gene> m_genes;
	unsigned int m_randomSeed;
	void Gen(unsigned int seed);
};


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
	Chromosome();
	Chromosome(unsigned int seed);
	//copy constructor
	Chromosome(const Chromosome& var);
	Chromosome& operator=(const Chromosome& var);
	int ChromSize() { return m_chromsize; }
	std::vector<Gene>& Genes();
	float m_fitness;
private:
	const int m_chromsize = 6;
	std::vector<Gene> m_genes;
	unsigned int m_randomSeed;
	void Gen(unsigned int seed);
};


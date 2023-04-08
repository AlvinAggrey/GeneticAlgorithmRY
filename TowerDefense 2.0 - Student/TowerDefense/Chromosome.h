#pragma once
#include <vector>
#include <random>

struct Gene
{
	//x and y
	int m_position[2] = { 0,0 };
	int m_towerType = 0;

	Gene() {};
	Gene(int x, int y, int towerType)
	{
		m_position[0] = x;
		m_position[1] = y;
		m_towerType = towerType;
	};
};

class Chromosome
{
public:
	//random_device is exspensive so pass it in
	Chromosome();
	Chromosome(unsigned int seed);
	void Init(int chromSize);

	//copy constructor
	Chromosome(const Chromosome& var);
	Chromosome& operator=(const Chromosome& var);

	int ChromSize() { return m_chromsize; }	void AddGene(Gene gene);
	void SetSeed(unsigned int seed);
	void ClearAll();
	std::vector<Gene>& Genes();
	float m_fitness;
private:
	int m_chromsize = 0;
	std::vector<Gene> m_genes;
	unsigned int m_randomSeed;
	void Gen(unsigned int seed);
};


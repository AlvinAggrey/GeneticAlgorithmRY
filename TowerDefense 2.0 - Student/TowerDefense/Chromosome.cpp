#include "Chromosome.h"
//requires seed = random_device num
Chromosome::Chromosome()
{
	std::random_device rd;
	m_randomSeed = rd();
	m_fitness = 1;
	Gen(m_randomSeed);
};
Chromosome::Chromosome(unsigned int seed)
{
	m_randomSeed = seed;
	m_fitness = 1;
	Gen(m_randomSeed);
}

void Chromosome::Gen(unsigned int seed)
{
	std::mt19937 gen(m_randomSeed); // pass seed in from true random generator (rd)
	//(0,0) to (25,17) 
	std::uniform_int_distribution<int> genPosX(0, 25);
	std::uniform_int_distribution<int> genPosY(0, 17);
	std::uniform_int_distribution<int> genTowerType(1, 3);

	for (int i = 0; i < m_chromsize; i++)
	{
		Gene gene;
		gene.m_position[0] = genPosX(gen);
		gene.m_position[1] = genPosY(gen);
		gene.m_towerType = genTowerType(gen);

		m_genes.push_back(gene);
	}
}

Chromosome::Chromosome(const Chromosome& var)
{
	m_fitness = var.m_fitness;
	m_randomSeed = var.m_randomSeed;
	m_genes = var.m_genes;
}

Chromosome& Chromosome::operator=(const Chromosome& var)
{
	m_fitness = var.m_fitness;
	m_randomSeed = var.m_randomSeed;
	m_genes = var.m_genes;

	return *this;
}

std::vector<Gene>& Chromosome::Genes()
{
	return m_genes;
}

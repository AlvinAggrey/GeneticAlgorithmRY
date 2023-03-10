#include "GAGenMaker.h"

GAGenMaker::GAGenMaker()
{
    m_selectionMethod = SelectionMethod::Roulette;
    m_crossoverMethod = CrossoverMethod::Onepoint;
    m_popSize = 2;
    m_childAmount = 1;

    m_gen.seed(m_rd());
}

GAGenMaker::GAGenMaker(SelectionMethod selectionMethod, CrossoverMethod crossoverMethod, int popSize, int childAmount)
{
    m_selectionMethod = selectionMethod;
    m_crossoverMethod = crossoverMethod;
    m_popSize = popSize;
    m_childAmount = childAmount;

    m_gen.seed(m_rd());
}
std::vector<individual> GAGenMaker::Selection(std::vector<individual> population)
{
    std::vector<individual> matingPool;

    switch (m_selectionMethod)
    {
    case SelectionMethod::Roulette:
    {
        matingPool = m_selector.RouletteSelection(population, 4);
        break;
    }
    }

    return matingPool;
}
std::vector<individual> GAGenMaker::Crossover(std::vector<individual> matingPool, int childAmount)
{
    std::vector<individual> children;
    for (int i = 0; i < childAmount; i++)
    {
        std::uniform_int_distribution<int> dist(0, matingPool.size() - 1);
        int index1 = dist(m_gen);
        int index2 = dist(m_gen);
        Chromosome parent1 = matingPool[index1].first;
        Chromosome parent2 = matingPool[index2].first;

        switch (m_crossoverMethod)
        {
        case CrossoverMethod::Onepoint:
        {
            Chromosome child = m_geneCrossover.OnepointCrossover(parent1, parent2);
            children.push_back(individual(child, false));
            break;
        }
        }
    }
    return children;
}
GAGenMaker::GAGenMaker(const GAGenMaker& other)
{
    m_selectionMethod = other.m_selectionMethod;
    m_crossoverMethod = other.m_crossoverMethod;
    m_popSize = other.m_popSize;
    m_childAmount = other.m_childAmount;
    m_gen = other.m_gen;
    m_geneCrossover = other.m_geneCrossover;
    m_selector = other.m_selector;
}
GAGenMaker GAGenMaker::operator=(const GAGenMaker& other)
{
    m_gen = other.m_gen;
    m_popSize = other.m_popSize;
    m_childAmount = other.m_childAmount;
    m_selector = other.m_selector;
    m_geneCrossover = other.m_geneCrossover;
    m_selectionMethod = other.m_selectionMethod;
    m_crossoverMethod = other.m_crossoverMethod;
    
    return *this;
}
std::vector<individual> GAGenMaker::CreateGen()
{
    std::vector<individual> gen;
    for (int i = 0; i < m_popSize; i++)
    {
        gen.push_back(individual(Chromosome(m_gen()), false));
    }

    return gen;
}
std::vector<individual> GAGenMaker::NextGen(std::vector<individual> population)
{
    std::vector<individual> nextGen;
    nextGen = Crossover( Selection(population), m_childAmount);
    
    for (int i = m_childAmount; i < m_popSize; i++)
    {
        nextGen.push_back(individual(Chromosome(m_gen()), false));
    }


    return nextGen;
}

void GAGenMaker::SetSelectionMethod(SelectionMethod selectionMethod)
{
    m_selectionMethod = selectionMethod;
}

void GAGenMaker::SetCrossoverMethod(CrossoverMethod crossovermethod)
{
    m_crossoverMethod = crossovermethod;
}


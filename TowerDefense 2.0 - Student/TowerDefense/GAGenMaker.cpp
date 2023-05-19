#include "GAGenMaker.h"

GAGenMaker::GAGenMaker()
{
    m_selectionMethod = SelectionMethod::Roulette;
    m_crossoverMethod = CrossoverMethod::OnePoint;
    m_popSize = 2;
    m_childAmount = 1;

    m_gen.seed(m_rd());
}

GAGenMaker::GAGenMaker(SelectionMethod selectionMethod, CrossoverMethod crossoverMethod, int popSize, int childAmount, int chromSize)
{
    m_selectionMethod = selectionMethod;
    m_crossoverMethod = crossoverMethod;
    m_popSize = popSize;
    m_childAmount = childAmount;
    m_chromSize = chromSize;
    m_gen.seed(m_rd());
}
//not completely perfect
Chromosome GAGenMaker::Mutate(Chromosome chrom)
{
    Chromosome mutatedChrom = chrom;
    Chromosome randChrom;
    randChrom.Init(m_chromSize);
    std::uniform_int_distribution<int> distGene(0, chrom.ChromSize());
    int geneElement = distGene(m_gen);
    std::uniform_int_distribution<int> dist(0, 3);
    //std::uniform_int_distribution<int> distTower(1, 4);

    switch (dist(m_gen))
    {
    case 0:
        mutatedChrom.Genes().at(geneElement).m_towerType = randChrom.Genes().at(0).m_towerType;
        break;
    case 1:
        mutatedChrom.Genes().at(geneElement).m_position[0] = randChrom.Genes().at(0).m_position[0];
        break;
    case 2:
        mutatedChrom.Genes().at(geneElement).m_position[1] = randChrom.Genes().at(0).m_position[1];
        break;
    }
    return mutatedChrom;
}
std::vector<individual> GAGenMaker::Selection(std::vector<individual> population)
{
    std::vector<individual> matingPool;

    switch (m_selectionMethod)
    {
    case SelectionMethod::Ranked:
    {
        matingPool = m_selector.Ranked(population, m_selectNum, m_matingPoolSize);
        break;
    }
    case SelectionMethod::Roulette:
    {
        matingPool = m_selector.Roulette(population,m_matingPoolSize);
        break;
    }
    case SelectionMethod::LinearRanked:
    {
        matingPool = m_selector.LinearRanked(population, m_bandDists);
        break;
    }
    case SelectionMethod::Elitist:
    {
        //matingPool = m_selector.Elitist(population, m_bandDists, 4);
        break;
    }
    case SelectionMethod::Stochastic:
    {
        matingPool = m_selector.StochasticSampling(population, m_matingPoolSize);
        break;
    }
    case SelectionMethod::Tournament:
    {
        matingPool = m_selector.Tournament(population, m_matingPoolSize, m_tournySize);
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

        Chromosome child;

        switch (m_crossoverMethod)
        {
        case CrossoverMethod::OnePoint:
        {
            child = m_geneCrossover.OnePoint(parent1, parent2);
            break;
        }
        case CrossoverMethod::TwoPoint:
        {
            child = m_geneCrossover.TwoPoint(parent1, parent2);
            break;
        }
        default:
            child = m_geneCrossover.OnePoint(parent1, parent2);
            break;
        }

        std::uniform_real_distribution<float> mutaionDist(0, 1);
        float chance = mutaionDist(m_gen);

        if (chance <= m_mutationChance)
        {
            child = Mutate(child);
        }
        children.push_back(individual(child, false));
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
    m_chromSize = other.m_chromSize;
    
    return *this;
}
std::vector<individual> GAGenMaker::CreateGen()
{
    std::vector<individual> gen;
    for (int i = 0; i < m_popSize; i++)
    {
        Chromosome chrom(m_gen());
        chrom.Init(m_chromSize);
        gen.push_back(individual(chrom, false));
    }

    return gen;
}
std::vector<individual> GAGenMaker::NextGen(std::vector<individual> population)
{
    std::vector<individual> nextGen;
    nextGen = Crossover( Selection(population), m_childAmount);

    for (int i = m_childAmount; i < m_popSize-1; i++)
    {
        Chromosome chrom(m_gen());
        chrom.Init(m_chromSize);
        nextGen.push_back(individual(chrom, false));
    }

    //elitist
    Chromosome elite;
    for (auto indiv : population)
    {
        if (indiv.first.m_fitness > elite.m_fitness)
        {
            elite = indiv.first;
        }
    }
    nextGen.push_back(individual(elite, false));

    return nextGen;
}

int GAGenMaker::GetChromSize()
{
    return m_chromSize;
}

//void GAGenMaker::SetSelectionMethod(SelectionMethod selectionMethod)
//{
//    m_selectionMethod = selectionMethod;
//}
//
//void GAGenMaker::SetCrossoverMethod(CrossoverMethod crossovermethod)
//{
//    m_crossoverMethod = crossovermethod;
//}

void GAGenMaker::UseRanked(int selectNum, int matingPoolSize)
{
    m_selectionMethod = SelectionMethod::Ranked;
    m_selectNum = selectNum;
    m_matingPoolSize = matingPoolSize;
}

void GAGenMaker::UseRoulette(int matingPoolSize)
{
    m_selectionMethod = SelectionMethod::Roulette;
    m_matingPoolSize = matingPoolSize;
}

void GAGenMaker::UseLinearRanked(std::vector<int> bandDists)
{
    m_selectionMethod = SelectionMethod::LinearRanked;
    m_bandDists = bandDists;
}

void GAGenMaker::UseElitist(std::vector<int> bandDists)
{
    m_selectionMethod = SelectionMethod::Elitist;
    m_bandDists = bandDists;
}

void GAGenMaker::UseTournament(int matingPoolSize, int tournySize)
{
    m_selectionMethod = SelectionMethod::Tournament;
    m_matingPoolSize = matingPoolSize;
    m_tournySize = tournySize;
}

void GAGenMaker::UseStochastic(int matingPoolSize, int tournySize)
{
    m_selectionMethod = SelectionMethod::Stochastic;
    m_tournySize = tournySize;
    m_matingPoolSize = matingPoolSize;
}

void GAGenMaker::UseOnePoint()
{
    m_crossoverMethod = CrossoverMethod::OnePoint;
}


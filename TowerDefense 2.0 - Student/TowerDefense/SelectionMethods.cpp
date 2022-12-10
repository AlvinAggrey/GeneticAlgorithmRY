#include "SelectionMethods.h"


std::vector<Chromosome> RankChroms(std::vector<Chromosome>* chroms)
{
    std::vector<Chromosome> rankedList;
    std::vector<Chromosome> tempList = *chroms;

    //order chromosomes by fitness biggest to lowests
    for (int i = 0; i < (int)chroms->size(); i++)
    {
        Chromosome fittest;
        int fittestIndex = 0;
        for (int i = 0; i < tempList.size(); i++)
        {
            if (i == 0)
            {
                fittest = tempList[i];
                fittestIndex = i;
            }
            else if (fittest.m_fitness < tempList[i].m_fitness)
            {
                fittest = tempList[i];
                fittestIndex = i;
            }

        }
        rankedList.push_back(fittest);
        tempList.erase(tempList.begin() + fittestIndex);
    }
    return rankedList;
}
//ranks generation, selects n from ranked list from the fittest, returns copies of selected individuals
std::vector<Chromosome> RankedSelection(std::vector<Chromosome>* gen, int selectNum, int parentNum)
{

    std::vector<Chromosome> rankedList = RankChroms(gen);

    //select n out of rankedList
    std::vector<Chromosome> selected;
    if (selectNum - 1 > rankedList.size())
    {
        std::cout << "Generation is too small to select this many indviduals." << std::endl;
        selectNum = rankedList.size();
    }
    for (int i = 0; i < selectNum; i++)
    {
        selected.push_back(rankedList[i]);
    }

    //add to selected chroms to mating pool - copies are allowed
    std::vector<Chromosome> matingPool;
    int selectedIndex = 0;
    do
    {
        matingPool.push_back(selected[selectedIndex]);

        selectedIndex++;

        if (selectedIndex >= selected.size())
        {
            selectedIndex = 0;
        }
    } while (matingPool.size() < parentNum);

    return matingPool;
}
//std::random_device rd;
Chromosome onepointCrossOver(Chromosome parent1, Chromosome parent2)
{
    Chromosome child = parent1;
    child.m_fitness = 1;

    std::mt19937 gen(1);
    std::uniform_int_distribution<int> dist(0, parent1.ChromSize());

    int randomPoint = dist(gen);
    //copy over second portion to the child
    std::copy(parent2.Genes().begin() + randomPoint, parent2.Genes().end(), child.Genes().begin() + randomPoint);

    return child;
}

//split generation into bands select n from each band
std::vector<Chromosome> LinearRankedSelection(std::vector<Chromosome>* gen, unsigned int seed, std::initializer_list<int> bandDists)
{
    std::vector<Chromosome> rankedList = RankChroms(gen);
    std::vector<Chromosome> matingPool;

    std::mt19937 mt(seed);

    int bandStart = 0;
    int bandDist;
    int randNum;
    //for as many bands as therre are
    for (int i = 0; i < bandDists.size(); i++)
    {
        //how many selected from current band?
        bandDist = *(bandDists.begin() + i);
        std::uniform_int_distribution<int> dist(bandStart, bandStart + bandDist - 1);
        for (int n = 0; n < bandDist; n++)
        {
            randNum = dist(mt);
            matingPool.push_back(rankedList[randNum]); //select indiv in current band randomly
        }
        bandStart += bandDist; //move to next lower fitness band
    }
    return matingPool;
}

//adds indivduals to returned mating pool - linear ranked selection
std::vector<Chromosome> ElitistSelection(std::vector<Chromosome>* gen, unsigned int seed, std::vector<Chromosome> indivs, std::initializer_list<int> bandDists)
{
    std::vector<Chromosome> matingPool = LinearRankedSelection(gen, seed, bandDists);
    if (indivs.size() != 0)
    {
        for (Chromosome indiv : indivs)
        {
            matingPool.push_back(indiv);
        }
    }
    return matingPool;
}

std::vector<Chromosome> TournamentSelection(std::vector<Chromosome>* gen, unsigned int seed, int matingPoolSize, int tournySize)
{
    std::vector<Chromosome> matingPool;
    std::vector<Chromosome> roster = *gen;
    std::vector<Chromosome> tournament;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> rosterIndex(0, roster.size() - 1);

    int randNum;

    for (int i = 0; i < matingPoolSize; i++)
    {
        //picks participants
        for (int i = 0; i < tournySize; i++)
        {
            randNum = rosterIndex(mt);
            tournament.push_back(roster[randNum]);
        }
        //pick tournament winner aka most fit
        matingPool.push_back(RankChroms(&tournament)[0]);
        tournament.clear();
    }
    return matingPool;
}
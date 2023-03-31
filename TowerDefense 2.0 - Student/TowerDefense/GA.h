#pragma once
#include <vector>
#include "Chromosome.h"
#include "GASelection.h"
#include "GAGenMaker.h"

//chromsome and completed testing?
typedef std::pair<Chromosome, bool> individual;

class GA
{
private:
    std::vector<individual> m_nextPopulation;
    std::vector<std::vector<individual>> m_generations;

    int m_popSize;
    int m_genAmount;
    int m_curGenIndex;
    int m_curIndivIndex;
    int m_childAmount;

    bool m_done;

    std::random_device m_rd;
    std::mt19937 m_gen;

    GAGenMaker m_genMaker;

    int m_selectNum;
    int m_matingPoolSize;
    int tournySize;
    std::initializer_list<int> m_bandDists;

    SelectionMethod m_selectionMethod;
    CrossoverMethod m_crossoverMethod;

public:
    GA ();
    GA(int gens, int popSize, int childAmount, SelectionMethod selectionMethod, CrossoverMethod crossoverMethod);
    
    GA(const GA& other);
    GA operator=(const GA& other);
    //Control GA
    void Init();
    void NextIndiv();
    void NextGen();

    //Set Properties
    void UseSLRanked(int selectNum, int matingPoolSize);
    void UseSLRoulette(int matingPoolSize);
    void UseSLLinearRanked(std::initializer_list<int> bandDists);
    void UseSLElitist(std::initializer_list<int> bandDists);
    void UseSLTournament(int matingPoolSize);
    void UseSLStochastic(int matingPoolSize, int tournySize);
    
    void UseCROnePoint();

    void SetCrossoverMethod(CrossoverMethod method);
    void SetSelectionMethod(SelectionMethod method);


    //Checks
    bool CheckTestAllPop();
    bool CheckIsDone();

    //Get Information
    individual& CurIndiv();
    std::vector<individual>& CurGen();
    int CurIndivIndex();
    int CurGenIndex();

    //Print
    void PrintGen(int index);
    void PrintAllGens();
};


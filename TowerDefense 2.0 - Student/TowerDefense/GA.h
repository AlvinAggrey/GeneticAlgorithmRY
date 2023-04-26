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
    int m_tournySize;
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
    //use epoch to make init gen
    void Init(std::vector<individual> epoch);
    void NextIndiv();
    void NextGen();
    //Set Properties
    void UseRanked(int selectNum, int matingPoolSize);
    void UseRoulette(int matingPoolSize);
    void UseLinearRanked(std::vector<int> bandDists);
    void UseElitist(std::vector<int> bandDists);
    void UseTournament(int matingPoolSize);
    void UseStochastic(int matingPoolSize, int tournySize);
    
    void UseOnePoint();

    //void SetCrossoverMethod(CrossoverMethod method);
    //void SetSelectionMethod(SelectionMethod method);


    //Checks
    bool CheckTestAllPop();
    bool CheckIsDone();

    //Get Information
    individual& CurIndiv();
    std::vector<individual>& CurGen();
    int CurIndivIndex();
    int CurGenIndex();
    int GetChromSize();

    //Print
    void PrintGen(int index);
    void PrintAllGens();
};


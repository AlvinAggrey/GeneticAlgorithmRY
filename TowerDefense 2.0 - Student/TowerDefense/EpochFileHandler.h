#pragma once
#include "Chromosome.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

//writes and reads Epochs to/from files
class EpochFileHandler
{
public:
    //Write
    void WriteGenToFile(std::vector<Chromosome> generation, std::string filename = "Epochs.txt");

    //Read
    std::vector<Chromosome> ReadGenFromFile(std::string filename);

private:
    Chromosome ExtractChromosome(std::string seq);

};


#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include <unordered_map>
#include "boost/filesystem.hpp"
#include "TH1D.h"
#include "RootObjectExtractor.hpp"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
    std::vector<TH1D> pobabilityDensities;
    double lithiumProbability;
    
  public:
    LikelihoodComputer(const boost::filesystem::path& densitiesFilePath, double lithiumProbability);
    LikelihoodComputer(std::unordered_map<std::string, TH1D> pobabilityDensityMap, double lithiumProbability);
    double getLikelihood(double distanceToTrack, unsigned numberOfFollowers);
    
  };
  
}

#endif
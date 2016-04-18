#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include <unordered_map>
#include "TH1D.h"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
    std::unordered_map<std::string, TH1D> pobabilityDensities;
    double lithiumProbability;
    
  public:
    LikelihoodComputer(std::unordered_map<std::string, TH1D> pobabilityDensities, double lithiumProbability);
    double getLikelihood(double distanceToTrack, unsigned numberOfFollowers);
    
  };
  
}

#endif
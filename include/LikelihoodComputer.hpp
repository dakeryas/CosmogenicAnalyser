#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include <unordered_map>
#include "boost/filesystem.hpp"
#include "TH1D.h"
#include "RootObjectExtractor.hpp"
#include "Utility.hpp"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
    std::vector<TH1D> probabilityDensities;
    double lithiumProbability;
    
  public:
    LikelihoodComputer(const boost::filesystem::path& densitiesFilePath, double lithiumProbability);
    template <class DensityIterator>
    LikelihoodComputer(DensityIterator densityBegin, DensityIterator densityEnd, double lithiumProbability);
    template <class DensityContainer>
    LikelihoodComputer(const DensityContainer& probabilityDensities, double lithiumProbability);
    double getLikelihood(double distanceToTrack, unsigned numberOfFollowers);
    
  };
  
  template <class DensityIterator>
  LikelihoodComputer::LikelihoodComputer(DensityIterator densityBegin, DensityIterator densityEnd, double lithiumProbability)
  :probabilityDensities(4),lithiumProbability(lithiumProbability){

    probabilityDensities[0] = Utility::getDistribution("muondist_sig", densityBegin, densityEnd);
    probabilityDensities[1] = Utility::getDistribution("numNeutrons_sig", densityBegin, densityEnd);
    probabilityDensities[2] = Utility::getDistribution("muondist_bkg", densityBegin, densityEnd);
    probabilityDensities[3] = Utility::getDistribution("numNeutrons_bkg", densityBegin, densityEnd);
    
  }
  
  template <class DensityContainer>
  LikelihoodComputer::LikelihoodComputer(const DensityContainer& probabilityDensities, double lithiumProbability)
  :LikelihoodComputer(std::begin(probabilityDensities), std::end(probabilityDensities), lithiumProbability){
    
  }
  
}

#endif
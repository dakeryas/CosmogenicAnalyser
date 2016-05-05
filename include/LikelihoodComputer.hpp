#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include <unordered_map>
#include "boost/filesystem.hpp"
#include "TH1.h"
#include "RootObjectExtractor.hpp"
#include "Utility.hpp"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
    double lithiumProbability;
    std::vector<std::unique_ptr<TH1>> probabilityDensities;
    
  public:
    LikelihoodComputer(double lithiumProbability, const boost::filesystem::path& densitiesFilePath);
    template <class DensityIterator>
    LikelihoodComputer(double lithiumProbability, DensityIterator densityBegin, DensityIterator densityEnd);
    template <class DensityContainer>
    LikelihoodComputer(double lithiumProbability, const DensityContainer& probabilityDensities);
    LikelihoodComputer(const LikelihoodComputer& other);
    LikelihoodComputer(LikelihoodComputer&& other) = default;
    LikelihoodComputer& operator = (LikelihoodComputer other);
    LikelihoodComputer& operator = (LikelihoodComputer&& other) = default;
    virtual ~LikelihoodComputer() = default;
    double getLikelihood(double distanceToTrack, unsigned numberOfFollowers);
    
  };
  
  template <class DensityIterator>
  LikelihoodComputer::LikelihoodComputer(double lithiumProbability, DensityIterator densityBegin, DensityIterator densityEnd)
  :lithiumProbability(lithiumProbability){

    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("muondist_sig", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("numNeutrons_sig", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("muondist_bkg", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("numNeutrons_bkg", densityBegin, densityEnd)));
    
  }
  
  template <class DensityContainer>
  LikelihoodComputer::LikelihoodComputer(double lithiumProbability, const DensityContainer& probabilityDensities)
  :LikelihoodComputer(lithiumProbability, std::begin(probabilityDensities), std::end(probabilityDensities)){
    
  }
  
}

#endif
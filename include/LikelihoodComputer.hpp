#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include <unordered_map>
#include "boost/filesystem.hpp"
#include "TH1.h"
#include "RootObjectExtractor.hpp"
#include "Utility.hpp"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
    double priorRatio;
    std::vector<std::unique_ptr<TH1>> probabilityDensities;
    
  public:
    LikelihoodComputer(double priorRatio, const boost::filesystem::path& densitiesFilePath);
    template <class DensityIterator>
    LikelihoodComputer(double priorRatio, DensityIterator densityBegin, DensityIterator densityEnd);
    template <class DensityContainer>
    LikelihoodComputer(double priorRatio, const DensityContainer& probabilityDensities);
    LikelihoodComputer(const LikelihoodComputer& other);
    LikelihoodComputer(LikelihoodComputer&& other) = default;
    LikelihoodComputer& operator = (LikelihoodComputer other);
    LikelihoodComputer& operator = (LikelihoodComputer&& other) = default;
    virtual ~LikelihoodComputer() = default;
    double getLikelihood(double distanceToTrack, unsigned numberOfFollowers);
    
  };
  
  template <class DensityIterator>
  LikelihoodComputer::LikelihoodComputer(double priorRatio, DensityIterator densityBegin, DensityIterator densityEnd)
  :priorRatio(priorRatio){

    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("muondist_sig", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("numNeutrons_sig", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("muondist_bkg", densityBegin, densityEnd)));
    probabilityDensities.emplace_back(std::make_unique<typename DensityIterator::value_type>(Utility::getDistribution("numNeutrons_bkg", densityBegin, densityEnd)));
    
  }
  
  template <class DensityContainer>
  LikelihoodComputer::LikelihoodComputer(double priorRatio, const DensityContainer& probabilityDensities)
  :LikelihoodComputer(priorRatio, std::begin(probabilityDensities), std::end(probabilityDensities)){
    
  }
  
}

#endif
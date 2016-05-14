#ifndef COSMOGENIC_ANALYSER_PAIR_SELECTOR_H
#define COSMOGENIC_ANALYSER_PAIR_SELECTOR_H

#include "Cosmogenic/CandidatePair.hpp"
#include "Cosmogenic/InnerVetoThreshold.hpp"
#include "Cosmogenic/ReconstructionCutParameters.hpp"
#include "Cosmogenic/BufferMuonCutParameters.hpp"

namespace CosmogenicAnalyser{

  template <class T>//to accept entries saved with the accuracy of type T
  class PairSelector{
    
    CosmogenicHunter::Bounds<T> promptEnergyBounds;
    CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold;
    CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters;
    CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters;
    T minChimneyInconsistencyRatio;
    
  public:
    PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold, CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters, CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters, T minChimneyInconsistencyRatio);
    bool tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const;
    
  };
  
  template <class T>
  PairSelector<T>::PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold, CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters, CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters, T minChimneyInconsistencyRatio)
  :promptEnergyBounds(std::move(promptEnergyBounds)),promptInnerVetoThreshold(std::move(promptInnerVetoThreshold)),bufferMuonCutParameters(std::move(bufferMuonCutParameters)),reconstructionCutParameters(std::move(reconstructionCutParameters)),minChimneyInconsistencyRatio(minChimneyInconsistencyRatio){
  
  }
  
  template <class T>
  bool PairSelector<T>::tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const{
    
    return candidatePair.getPrompt().hasVisibleEnergyWithin(promptEnergyBounds) && !(candidatePair.isVetoed(promptInnerVetoThreshold) || candidatePair.isPoorlyReconstructed(reconstructionCutParameters) || candidatePair.isBufferMuon(bufferMuonCutParameters) || candidatePair.isStoppingMuon(minChimneyInconsistencyRatio));
    
  }
  
}

#endif
#ifndef COSMOGENIC_ANALYSER_PAIR_SELECTOR_H
#define COSMOGENIC_ANALYSER_PAIR_SELECTOR_H

#include "Cosmogenic/CandidatePair.hpp"
#include "Cosmogenic/InnerVetoThreshold.hpp"
#include "Cosmogenic/ReconstructionCutParameters.hpp"
#include "Cosmogenic/BufferMuonCutParameters.hpp"
#include "Cosmogenic/ChimneyVeto.hpp"

namespace CosmogenicAnalyser{

  template <class T>//to accept entries saved with the accuracy of type T
  class PairSelector{
    
    CosmogenicHunter::Bounds<T> promptEnergyBounds;
    CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold;
    CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters;
    CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters;
    CosmogenicHunter::ChimneyVeto<T> chimneyVeto;
    
  public:
    PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold, CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters, CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters, CosmogenicHunter::ChimneyVeto<T> chimneyVeto);
    bool tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const;
    
  };
  
  template <class T>
  PairSelector<T>::PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVetoThreshold<T> promptInnerVetoThreshold, CosmogenicHunter::BufferMuonCutParameters<T> bufferMuonCutParameters, CosmogenicHunter::ReconstructionCutParameters<T> reconstructionCutParameters, CosmogenicHunter::ChimneyVeto<T> chimneyVeto)
  :promptEnergyBounds(std::move(promptEnergyBounds)),promptInnerVetoThreshold(std::move(promptInnerVetoThreshold)),bufferMuonCutParameters(std::move(bufferMuonCutParameters)),reconstructionCutParameters(std::move(reconstructionCutParameters)),chimneyVeto(std::move(chimneyVeto)){
  
  }
  
  template <class T>
  bool PairSelector<T>::tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const{
    
    return candidatePair.getPrompt().hasVisibleEnergyWithin(promptEnergyBounds) && !(candidatePair.isVetoed(promptInnerVetoThreshold) || candidatePair.isPoorlyReconstructed(reconstructionCutParameters) || candidatePair.isBufferMuon(bufferMuonCutParameters) || candidatePair.isStoppingMuon(chimneyVeto));
    
  }
  
}

#endif
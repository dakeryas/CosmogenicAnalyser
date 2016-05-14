#ifndef COSMOGENIC_ANALYSER_PAIR_SELECTOR_H
#define COSMOGENIC_ANALYSER_PAIR_SELECTOR_H

#include "Cosmogenic/CandidatePair.hpp"
#include "Cosmogenic/InnerVeto.hpp"
#include "Cosmogenic/ReconstructionVeto.hpp"
#include "Cosmogenic/BufferMuonVeto.hpp"
#include "Cosmogenic/ChimneyVeto.hpp"

namespace CosmogenicAnalyser{

  template <class T>//to accept entries saved with the accuracy of type T
  class PairSelector{
    
    CosmogenicHunter::Bounds<T> promptEnergyBounds;
    CosmogenicHunter::InnerVeto<T> promptInnerVeto;
    CosmogenicHunter::BufferMuonVeto<T> bufferMuonVeto;
    CosmogenicHunter::ReconstructionVeto<T> reconstructionVeto;
    CosmogenicHunter::ChimneyVeto<T> chimneyVeto;
    
  public:
    PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVeto<T> promptInnerVeto, CosmogenicHunter::BufferMuonVeto<T> bufferMuonVeto, CosmogenicHunter::ReconstructionVeto<T> reconstructionVeto, CosmogenicHunter::ChimneyVeto<T> chimneyVeto);
    bool tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const;
    
  };
  
  template <class T>
  PairSelector<T>::PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, CosmogenicHunter::InnerVeto<T> promptInnerVeto, CosmogenicHunter::BufferMuonVeto<T> bufferMuonVeto, CosmogenicHunter::ReconstructionVeto<T> reconstructionVeto, CosmogenicHunter::ChimneyVeto<T> chimneyVeto)
  :promptEnergyBounds(std::move(promptEnergyBounds)),promptInnerVeto(std::move(promptInnerVeto)),bufferMuonVeto(std::move(bufferMuonVeto)),reconstructionVeto(std::move(reconstructionVeto)),chimneyVeto(std::move(chimneyVeto)){
  
  }
  
  template <class T>
  bool PairSelector<T>::tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const{
    
    return candidatePair.getPrompt().hasVisibleEnergyWithin(promptEnergyBounds) && !(candidatePair.isVetoed(promptInnerVeto) || candidatePair.isPoorlyReconstructed(reconstructionVeto) || candidatePair.isBufferMuon(bufferMuonVeto) || candidatePair.isStoppingMuon(chimneyVeto));
    
  }
  
}

#endif
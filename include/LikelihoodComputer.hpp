#ifndef COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H
#define COSMOGENIC_ANALYSER_LIKELIHOOD_COMPUTER_H

#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Single.hpp"
#include "Cosmogenic/Shower.hpp"

namespace CosmogenicAnalyser{
  
  class LikelihoodComputer{
    
  public:
    LikelihoodComputer() = default;
    template <class T, class K>
    double getLikelihood(const CosmogenicHunter::Single<T>& prompt, const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower) const;
    
  };
  
  template <class T, class K>
  double LikelihoodComputer::getLikelihood(const CosmogenicHunter::Single<T>& prompt, const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower) const{
    
    if(muonShower.getNumberOfFollowers() > 0) return 0.7;
    
    auto distance = CosmogenicHunter::getDistanceBetween(prompt, muonShower.getInitiator());
    if(distance < 5e2) return 0.5;
    else if(distance < 5e3) return 0.4;
    else if(distance < 1e3) return 0.2;
    else if(distance < 1.5e3) return 0.1;
    else return 0.05;
    
  }

}

#endif
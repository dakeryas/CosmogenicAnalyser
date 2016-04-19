#ifndef COSMOGENIC_ANALYSER_NEUTRON_DISTRIBUTIONS_H
#define COSMOGENIC_ANALYSER_NEUTRON_DISTRIBUTIONS_H

#include "TH1F.h"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Single.hpp"
#include "Cosmogenic/Shower.hpp"
#include "Binning.hpp"

namespace CosmogenicAnalyser{

  class MuonShowerAnalyser{
    
    std::vector<TH1F> distributions;
    std::unordered_set<unsigned> formerNeutronIdentifiers;//to avoid neutron double-counting
    
  public:
    MuonShowerAnalyser(const Binning& distanceBinning, const Binning& energyBinning);
    template <class T, class K>
    void analyse(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower);
    const TH1F& getDistribution(const std::string& distributionName) const;
    const std::vector<TH1F>& getDistributions() const;
    void resetDistributions();
    void resetIdentifiers();
    
  };
  
  template <class T, class K>
  void MuonShowerAnalyser::analyse(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower){
    
    for(const auto& follower : muonShower.getFollowerWindow()){
      
      distributions[0].Fill(CosmogenicHunter::getDistanceBetween(follower, muonShower.getInitiator()));
      if(formerNeutronIdentifiers.insert(follower.getIdentifier()).second == true) distributions[1].Fill(follower.getVisibleEnergy());
      
    }
    
  }
  
}

#endif
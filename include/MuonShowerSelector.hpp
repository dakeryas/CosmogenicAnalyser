#ifndef COSMOGENIC_ANALYSER_MUON_SHOWER_SELECTOR_H
#define COSMOGENIC_ANALYSER_MUON_SHOWER_SELECTOR_H

#include "Cosmogenic/MuonDefinition.hpp"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Single.hpp"
#include "Cosmogenic/Shower.hpp"

namespace CosmogenicAnalyser{

  template <class Initiator>
  class MuonShowerSelector{
    
    CosmogenicHunter::MuonDefinition<Initiator> muonDefinition;
    unsigned neutronMultiplicityThreshold;
    
  public:
    MuonShowerSelector(CosmogenicHunter::MuonDefinition<Initiator> muonDefinition, int neutronMultiplicityThreshold);
    const CosmogenicHunter::MuonDefinition<Initiator>& getMuonDefinition() const;
    unsigned getNeutronMultiplicityThreshold() const;
    template <class Follower>
    bool tag(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<Initiator>, CosmogenicHunter::Single<Follower>>& muonShower) const;
    Initiator getVisibleEnergy(const CosmogenicHunter::Muon<Initiator>& muon) const;
    
  };
  
  template <class Initiator>
  MuonShowerSelector<Initiator>::MuonShowerSelector(CosmogenicHunter::MuonDefinition<Initiator> muonDefinition, int neutronMultiplicityThreshold)
  :muonDefinition(std::move(muonDefinition)),neutronMultiplicityThreshold(neutronMultiplicityThreshold){
    
    if(neutronMultiplicityThreshold < 0) throw std::invalid_argument(std::to_string(neutronMultiplicityThreshold)+"is not a valid neutron multiplicity.");
  
  }
  
  template <class Initiator>
  const CosmogenicHunter::MuonDefinition<Initiator>& MuonShowerSelector<Initiator>::getMuonDefinition() const{

    return muonDefinition;

  }
  
  template <class Initiator>
  unsigned MuonShowerSelector<Initiator>::getNeutronMultiplicityThreshold() const{

    return neutronMultiplicityThreshold;

  }
  
  template <class Initiator>
  template <class Follower>
  bool MuonShowerSelector<Initiator>::tag(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<Initiator>, CosmogenicHunter::Single<Follower>>& muonShower) const{
    
    return muonShower.getNumberOfFollowers() >= neutronMultiplicityThreshold && muonDefinition.tag(muonShower.getInitiator());
    
  }
  
  template <class Initiator>
  Initiator MuonShowerSelector<Initiator>::getVisibleEnergy(const CosmogenicHunter::Muon<Initiator>& muon) const{

    return muonDefinition.getVisibleEnergy(muon.getDetectorCharge());

  }
  
  template <class Initiator>
  std::ostream& operator<<(std::ostream& output, const MuonShowerSelector<Initiator>& muonShowerSelector){
    
    auto formerPrecision = output.precision();
    
    output<<muonShowerSelector.getMuonDefinition()<<"\n"
      <<std::setw(26)<<std::left<<"Neutron threshold"<<": "<<std::setw(8)<<std::right<<std::setprecision(0)<<muonShowerSelector.getNeutronMultiplicityThreshold();
      
    output.unsetf(std::ios_base::floatfield);
    output<<std::setprecision(formerPrecision);
    
    return output;
    
  }
  
}

#endif
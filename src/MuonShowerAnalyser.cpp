#include "MuonShowerAnalyser.hpp"

namespace CosmogenicAnalyser{

  MuonShowerAnalyser::MuonShowerAnalyser(const Binning& distanceBinning, const Binning& energyBinning):distributions(2){
    
    distributions[0] = TH1F("neutronDistanceToTrack", "neutron distance to track", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions[1] = TH1F("neutronEnergySpectrum", "neutron energy spectrum", energyBinning.numberOfBins, energyBinning.lowerEdge, energyBinning.upperEdge);
    
    for(auto& hist : distributions) hist.Sumw2();
    
  }
  
  const TH1F& MuonShowerAnalyser::getDistribution(const std::string& distributionName) const{
    
    auto it = std::find_if(distributions.begin(), distributions.end(), [&](const auto& distribution){return distribution.GetName() == distributionName;});
    if(it != distributions.end()) return *it;
    else throw std::runtime_error(distributionName+" is not a known distribution.");
    
  }
  
  const std::vector<TH1F>& MuonShowerAnalyser::getDistributions() const{
    
    return distributions;
    
  }
  
  void MuonShowerAnalyser::resetDistributions(){
    
    for(auto& hist : distributions) hist.Reset();
    
  }
  
  void MuonShowerAnalyser::resetIdentifiers(){
    
    formerNeutronIdentifiers.clear();
    
  }
  
}

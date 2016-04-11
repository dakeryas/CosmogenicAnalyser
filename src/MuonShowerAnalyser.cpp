#include "MuonShowerAnalyser.hpp"

namespace CosmogenicAnalyser{

  MuonShowerAnalyser::MuonShowerAnalyser(const Binning& energyBinning, const Binning& distanceBinning){
    
    distributions["neutronEnergySpectrum"] = TH1F("neutronEnergySpectrum", "neutron energy spectrum", energyBinning.numberOfBins, energyBinning.lowerEdge, energyBinning.upperEdge);
    distributions["neutronDistanceToTrack"] = TH1F("neutronDistanceToTrack", "neutron distance to track", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    
    for(auto& histPair : distributions) histPair.second.Sumw2();
    
  }
  
  const TH1F& MuonShowerAnalyser::getDistribution(const std::string& distributionName) const{
    
    return distributions.at(distributionName);
    
  }
  
  const std::unordered_map<std::string, TH1F>& MuonShowerAnalyser::getDistributions() const{
    
    return distributions;
    
  }
  
  void MuonShowerAnalyser::resetDistributions(){
    
    for(auto& histPair : distributions) histPair.second.Reset();
    
  }
  
  void MuonShowerAnalyser::resetIdentifiers(){
    
    formerNeutronIdentifiers.clear();
    
  }
  
}

#include "MuonShowerAnalyser.hpp"
#include "Utility.hpp"

namespace CosmogenicAnalyser{

  MuonShowerAnalyser::MuonShowerAnalyser(const Binning& distanceBinning, const Binning& energyBinning):distributions(2){
    
    distributions[0] = TH1F("neutronDistanceToTrack", "neutron distance to track", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions[1] = TH1F("neutronEnergySpectrum", "neutron energy spectrum", energyBinning.numberOfBins, energyBinning.lowerEdge, energyBinning.upperEdge);
    
    for(auto& hist : distributions) hist.Sumw2();
    
  }
  
  const TH1F& MuonShowerAnalyser::getDistribution(const std::string& distributionName) const{
    
    return Utility::getDistribution(distributionName, distributions);
    
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

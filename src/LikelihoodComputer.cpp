#include "LikelihoodComputer.hpp"
#include <iostream>

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(std::unordered_map<std::string, TH1D> pobabilityDensities, double lithiumProbability)
  :pobabilityDensities(std::move(pobabilityDensities)),lithiumProbability(lithiumProbability){

    for(const auto& pobabilityDensityName : {"cosmogenic_distance", "cosmogenic_neutron", "background_distance", "background_neutron"})
      if(this->pobabilityDensities.find(pobabilityDensityName) == this->pobabilityDensities.end())
	throw std::invalid_argument("Required probability density '"+std::string(pobabilityDensityName)+"' cannot be found.");
    
  }

  double LikelihoodComputer::getLikelihood(double distanceToTrack, unsigned numberOfFollowers){
    
    int cosmogenicNeutronBinNumber = pobabilityDensities["cosmogenic_neutron"].FindFixBin(numberOfFollowers);
    int backgroundNeutronBinNumber = pobabilityDensities["background_neutron"].FindFixBin(numberOfFollowers);
    
    auto cosmogenicDensity = pobabilityDensities["cosmogenic_distance"].Interpolate(distanceToTrack) * pobabilityDensities["cosmogenic_neutron"].GetBinContent(cosmogenicNeutronBinNumber) * lithiumProbability;
    auto backgroundDensity = pobabilityDensities["background_distance"].Interpolate(distanceToTrack) * pobabilityDensities["background_neutron"].GetBinContent(backgroundNeutronBinNumber); //assumes background probability is 1
    
    if(cosmogenicDensity + backgroundDensity < 0) return 0;
    else return cosmogenicDensity / (cosmogenicDensity + backgroundDensity);
    
  }
  
}

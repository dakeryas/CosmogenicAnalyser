#include "LikelihoodComputer.hpp"
#include <iostream>

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(std::unordered_map<std::string, TH1D> pobabilityDensityMap, double lithiumProbability)
  :pobabilityDensities(pobabilityDensityMap.size()),lithiumProbability(lithiumProbability){

    for(const auto& pobabilityDensityName : {"cosmogenic_distance", "cosmogenic_neutron", "background_distance", "background_neutron"})
      if(pobabilityDensityMap.find(pobabilityDensityName) == pobabilityDensityMap.end())
	throw std::invalid_argument("Required probability density '"+std::string(pobabilityDensityName)+"' cannot be found.");
      
    pobabilityDensities[0] = pobabilityDensityMap["cosmogenic_distance"];
    pobabilityDensities[1] = pobabilityDensityMap["cosmogenic_neutron"];
    pobabilityDensities[2] = pobabilityDensityMap["background_distance"];
    pobabilityDensities[3] = pobabilityDensityMap["background_neutron"];
    
  }

  double LikelihoodComputer::getLikelihood(double distanceToTrack, unsigned numberOfFollowers){
    
    int cosmogenicNeutronBinNumber = pobabilityDensities[1].FindFixBin(numberOfFollowers);
    int backgroundNeutronBinNumber = pobabilityDensities[1+2].FindFixBin(numberOfFollowers);
    
    auto cosmogenicDensity = pobabilityDensities[0].Interpolate(distanceToTrack) * pobabilityDensities[1].GetBinContent(cosmogenicNeutronBinNumber) * lithiumProbability;
    auto backgroundDensity = pobabilityDensities[0+2].Interpolate(distanceToTrack) * pobabilityDensities[1+2].GetBinContent(backgroundNeutronBinNumber); //assumes background probability is 1
    
    if(cosmogenicDensity + backgroundDensity < 0) return 0;
    else return cosmogenicDensity / (cosmogenicDensity + backgroundDensity);
    
  }
  
}

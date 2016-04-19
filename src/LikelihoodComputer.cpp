#include "LikelihoodComputer.hpp"
#include <iostream>

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(const boost::filesystem::path& densitiesFilePath, double lithiumProbability)
  :pobabilityDensities(4),lithiumProbability(lithiumProbability){

    TFile densitiesFile(densitiesFilePath.c_str());
    pobabilityDensities[0] = RootObjectExtractor<TH1D>::extract("muondist_sig", densitiesFile);
    pobabilityDensities[1] = RootObjectExtractor<TH1D>::extract("numNeutrons_sig", densitiesFile);
    pobabilityDensities[2] = RootObjectExtractor<TH1D>::extract("muondist_bkg", densitiesFile);
    pobabilityDensities[3] = RootObjectExtractor<TH1D>::extract("numNeutrons_bkg", densitiesFile);
    
  }
  
  LikelihoodComputer::LikelihoodComputer(std::unordered_map<std::string, TH1D> pobabilityDensityMap, double lithiumProbability)
  :pobabilityDensities(pobabilityDensityMap.size()),lithiumProbability(lithiumProbability){

    for(const auto& pobabilityDensityName : {"muondist_sig", "numNeutrons_sig", "muondist_bkg", "numNeutrons_bkg"})
      if(pobabilityDensityMap.find(pobabilityDensityName) == pobabilityDensityMap.end())
	throw std::invalid_argument("Required probability density '"+std::string(pobabilityDensityName)+"' cannot be found.");
      
    pobabilityDensities[0] = pobabilityDensityMap["muondist_sig"];
    pobabilityDensities[1] = pobabilityDensityMap["numNeutrons_sig"];
    pobabilityDensities[2] = pobabilityDensityMap["muondist_bkg"];
    pobabilityDensities[3] = pobabilityDensityMap["numNeutrons_bkg"];
    
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

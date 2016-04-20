#include "LikelihoodComputer.hpp"

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(const boost::filesystem::path& densitiesFilePath, double lithiumProbability)
  :pobabilityDensities(4),lithiumProbability(lithiumProbability){

    TFile densitiesFile(densitiesFilePath.c_str());
    pobabilityDensities[0] = RootObjectExtractor<TH1D>::extract("muondist_sig", densitiesFile);
    pobabilityDensities[1] = RootObjectExtractor<TH1D>::extract("numNeutrons_sig", densitiesFile);
    pobabilityDensities[2] = RootObjectExtractor<TH1D>::extract("muondist_bkg", densitiesFile);
    pobabilityDensities[3] = RootObjectExtractor<TH1D>::extract("numNeutrons_bkg", densitiesFile);
    
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

#include "LikelihoodComputer.hpp"

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(const boost::filesystem::path& densitiesFilePath, double lithiumProbability)
  :probabilityDensities(4),lithiumProbability(lithiumProbability){

    TFile densitiesFile(densitiesFilePath.c_str());
    probabilityDensities[0] = RootObjectExtractor<TH1D>::extract("muondist_sig", densitiesFile);
    probabilityDensities[1] = RootObjectExtractor<TH1D>::extract("numNeutrons_sig", densitiesFile);
    probabilityDensities[2] = RootObjectExtractor<TH1D>::extract("muondist_bkg", densitiesFile);
    probabilityDensities[3] = RootObjectExtractor<TH1D>::extract("numNeutrons_bkg", densitiesFile);
    
  }
  
  double LikelihoodComputer::getLikelihood(double distanceToTrack, unsigned numberOfFollowers){
    
    int cosmogenicNeutronBinNumber = probabilityDensities[1].FindFixBin(numberOfFollowers);
    int backgroundNeutronBinNumber = probabilityDensities[1+2].FindFixBin(numberOfFollowers);
    
    auto cosmogenicDensity = probabilityDensities[0].Interpolate(distanceToTrack) * probabilityDensities[1].GetBinContent(cosmogenicNeutronBinNumber) * lithiumProbability;
    auto backgroundDensity = probabilityDensities[0+2].Interpolate(distanceToTrack) * probabilityDensities[1+2].GetBinContent(backgroundNeutronBinNumber); //assumes background probability is 1
    
    if(cosmogenicDensity + backgroundDensity < 0) return 0;
    else return cosmogenicDensity / (cosmogenicDensity + backgroundDensity);
    
  }
  
}

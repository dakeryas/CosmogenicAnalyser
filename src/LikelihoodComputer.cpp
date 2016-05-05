#include "LikelihoodComputer.hpp"

namespace CosmogenicAnalyser{
  
  LikelihoodComputer::LikelihoodComputer(double lithiumProbability, const boost::filesystem::path& densitiesFilePath)
  :lithiumProbability(lithiumProbability){

    TFile densitiesFile(densitiesFilePath.c_str());
    TH1::AddDirectory(false); 
    probabilityDensities.emplace_back(RootObjectExtractor<TH1*>::extract("muondist_sig", densitiesFile));
    probabilityDensities.emplace_back(RootObjectExtractor<TH1*>::extract("numNeutrons_sig", densitiesFile));
    probabilityDensities.emplace_back(RootObjectExtractor<TH1*>::extract("muondist_bkg", densitiesFile));
    probabilityDensities.emplace_back(RootObjectExtractor<TH1*>::extract("numNeutrons_bkg", densitiesFile));
    
  }
  
  LikelihoodComputer::LikelihoodComputer(const LikelihoodComputer& other):lithiumProbability(other.lithiumProbability){
    
    for(const auto& probabilityDensityPtr : other.probabilityDensities) probabilityDensities.emplace_back(dynamic_cast<TH1*>(probabilityDensityPtr->Clone()));

  }
  
  LikelihoodComputer& LikelihoodComputer::operator = (LikelihoodComputer other){
    
    this->lithiumProbability = other.lithiumProbability;
    std::swap(probabilityDensities, other.probabilityDensities);
    return *this;
    
  }
  
  double LikelihoodComputer::getLikelihood(double distanceToTrack, unsigned numberOfFollowers){
    
    int cosmogenicNeutronBinNumber = probabilityDensities[1]->FindFixBin(numberOfFollowers);
    int backgroundNeutronBinNumber = probabilityDensities[1+2]->FindFixBin(numberOfFollowers);
    
    auto cosmogenicDensity = probabilityDensities[0]->Interpolate(distanceToTrack) * probabilityDensities[1]->GetBinContent(cosmogenicNeutronBinNumber) * lithiumProbability;
    auto backgroundDensity = probabilityDensities[0+2]->Interpolate(distanceToTrack) * probabilityDensities[1+2]->GetBinContent(backgroundNeutronBinNumber); //assumes background probability is 1
    
    if(cosmogenicDensity + backgroundDensity < 0) return 0;
    else return cosmogenicDensity / (cosmogenicDensity + backgroundDensity);
    
  }
  
}

#include "CandidateMuonPairAnalyser.hpp"

#include "TF1.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TablePrinter.hpp"

namespace CosmogenicAnalyser{
  
  CandidateMuonPairAnalyser::CandidateMuonPairAnalyser(TimeDivision timeDivision, const Binning& distanceBinning, const Binning& neutronMultiplicityBinning)
  :timeDivision(std::move(timeDivision)){
    
    distributions["timeIntervals"] = TH1D("timeIntervals", "#DeltaT distribution", timeDivision.getNumberOfBins(), 0, timeDivision.getAnalysisTime());
    distributions["onTimeDistance"] = TH1D("onTimeDistanceDistribution", "on-time distance distribution", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions["onTimeNeutronMultiplicity"] = TH1D("onTimeNeutronMultiplicity", "on-time neutron multiplicity",  neutronMultiplicityBinning.numberOfBins, neutronMultiplicityBinning.lowerEdge, neutronMultiplicityBinning.upperEdge);
    distributions["offTimeDistance"] = TH1D("offTimeDistanceDistribution", "off-time distance distribution", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions["offTimeNeutronMultiplicity"] = TH1D("offTimeNeutronMultiplicity", "off-time neutron multiplicity", neutronMultiplicityBinning.numberOfBins, neutronMultiplicityBinning.lowerEdge, neutronMultiplicityBinning.upperEdge);
    
    for(auto& histPair : distributions) histPair.second.Sumw2();
    
    distributions.at("timeIntervals").GetXaxis()->SetTitle("time to previous #mu (ms)");
    distributions.at("timeIntervals").GetXaxis()->SetTitleOffset(1.2);
    std::string yTitle = "Entries (/"+std::to_string(static_cast<unsigned>(distributions.at("timeIntervals").GetBinWidth(1)))+"ms)";
    distributions.at("timeIntervals").GetYaxis()->SetTitle(yTitle.c_str());
    
  }
  
  const TimeDivision& CandidateMuonPairAnalyser::getTimeDivision() const{
    
    return timeDivision;
    
  }
  
  const TH1D& CandidateMuonPairAnalyser::getDistribution(const std::string& distributionName) const{
    
    return distributions.at(distributionName);
    
  }
  
  const std::unordered_map<std::string, TH1D>& CandidateMuonPairAnalyser::getDistributions() const{
    
    return distributions;
    
  }
  
  std::unordered_map<std::string, TH1D> CandidateMuonPairAnalyser::getBackgroundSubtractedDistributions() const{
    
    std::unordered_map<std::string, TH1D> backgroundSubtractedDistributions;
    
    backgroundSubtractedDistributions["backgroundSubtractedDistance"] = distributions.at("onTimeDistance") - (timeDivision.getOnTimeWindowLength() / timeDivision.getOffTimeWindowLength()) * distributions.at("offTimeDistance");
    backgroundSubtractedDistributions["backgroundSubtractedNeutronMultiplicity"] = distributions.at("onTimeNeutronMultiplicity") - (timeDivision.getOnTimeWindowLength() / timeDivision.getOffTimeWindowLength()) * distributions.at("offTimeNeutronMultiplicity");
    
    return backgroundSubtractedDistributions;
    
  }
  
  void CandidateMuonPairAnalyser::setTimeDivision(TimeDivision timeDivision){
    
    this->timeDivision = std::move(timeDivision);
    
  }
  
  void CandidateMuonPairAnalyser::reset(){
    
    for(auto& histPair : distributions) histPair.second.Reset();
    
  }
  
  TimeIntervalsFitResults CandidateMuonPairAnalyser::fitTimeIntervals(double lifetime, double numberOfNeutrinos, double muonRate){
    
    if(distributions.at("timeIntervals").GetNbinsX() > 0){
    
      muonRate *= Converter::millisecondsToSeconds(1);// convert s^-1 into ms^-1
      auto timebinWidth = distributions.at("timeIntervals").GetBinWidth(1);
      
      TF1 fitFunction("fitFunction", [&](double* x, double* p){
	return timebinWidth * (p[0]/lifetime*std::exp(-x[0]/lifetime) + p[1]*muonRate-std::pow(Converter::millisecondsToSeconds(p[2]),2)*x[0]);
      }, 0, timeDivision.getAnalysisTime(), 3);
      fitFunction.SetNpx(10*distributions.at("timeIntervals").GetNbinsX());
      fitFunction.SetParameters(distributions.at("timeIntervals").GetBinContent(1), numberOfNeutrinos, 0);
      fitFunction.SetParNames("Cosmogenics", "Neutrinos", "Accidental rate");
      
      TCanvas can{};//otherwise fitting triggers a TCanvas creating warning...
      auto fitResults = *distributions.at("timeIntervals").Fit(&fitFunction, "QRS").Get();
      TimeIntervalsFitResults timeIntervalsFitResults{
	{fitResults.Parameter(0), std::sqrt(fitResults.CovMatrix(0,0))}, 
	{fitResults.Parameter(1), std::sqrt(fitResults.CovMatrix(1,1))}, 
	{fitResults.Parameter(2), std::sqrt(fitResults.CovMatrix(2,2))},
	{fitResults.Chi2(), fitResults.Ndf()}
      };
      fitFunction.SetParameters(fitResults.Parameter(0), fitResults.Parameter(1));
      
      return timeIntervalsFitResults;
      
    }
    else throw std::runtime_error(std::string("Histogram ")+distributions.at("timeIntervals").GetName()+" is empty!");
    
  }
  
}
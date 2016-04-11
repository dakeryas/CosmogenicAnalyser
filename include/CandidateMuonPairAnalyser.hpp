#ifndef COSMOGENIC_ANALYSER_CANDIDATE_MUON_PAIR_ANALYSER_H
#define COSMOGENIC_ANALYSER_CANDIDATE_MUON_PAIR_ANALYSER_H

#include <unordered_map>
#include "TH1D.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "LikelihoodComputer.hpp"
#include "MaxWindow.hpp"
#include "Converter.hpp"
#include "TimeDivision.hpp"
#include "Binning.hpp"
#include "TimeIntervalsFitResults.hpp"

namespace CosmogenicAnalyser{
  
  template <class T>
  class CandidateMuonPairAnalyser{
    
    LikelihoodComputer likelihoodComputer;
    double likelihoodCut;
    std::vector<MaxWindow<double>> likelihoodWindows;
    std::unordered_map<std::string, TH1D> distributions;
    const CosmogenicHunter::Single<T>* prompt;//observer, not owning the pointed value
    
    void updateSpectra();
    
  public:  
    CandidateMuonPairAnalyser(double likelihoodCut, const TimeDivision& timeDivision, const Binning& distanceBinning, const Binning& neutronMultiplicityBinning, const Binning& energyBinning);
    double getLikeLihoodCut() const;
    double getOnTimeDuration() const;
    double getOffTimeDuration() const;
    double getSpannedAnalysisDuration() const;
    const TH1D& getDistribution(const std::string& distributionName) const;
    const std::unordered_map<std::string, TH1D>& getDistributions() const;
    std::unordered_map<std::string, TH1D> getBackgroundSubtractedDistributions() const;
    void setLikeLihoodCut(double LikelihoodCut);
    void setTimeDivision(const TimeDivision& timeDivision);
    void setPrompt(const CosmogenicHunter::Single<T>& prompt);
    template <class K>
    void analyse(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower);
    void updateDistributions();
    void resetDistributions();
    TimeIntervalsFitResults fitTimeIntervals(double lifetime, double numberOfNeutrinos, double muonRate);
    
  };
  
  template <class T>
  void CandidateMuonPairAnalyser<T>::updateSpectra(){
    
    if(likelihoodWindows.front().getValue() > likelihoodCut) distributions["onTimeSpectrum"].Fill(prompt->getVisibleEnergy());
    double count = std::count_if(likelihoodWindows.begin() + 1, likelihoodWindows.end(), [&](const auto& window){return window.getValue() > likelihoodCut;});
    distributions["offTimeSpectrum"].Fill(prompt->getVisibleEnergy(), count); //count only off-time windows

  }
  
  template <class T>
  CandidateMuonPairAnalyser<T>::CandidateMuonPairAnalyser(double likelihoodCut, const TimeDivision& timeDivision, const Binning& distanceBinning, const Binning& neutronMultiplicityBinning, const Binning& energyBinning)
  :likelihoodCut(likelihoodCut),likelihoodWindows(timeDivision.getEqualLengthWindows<MaxWindow<double>>()),prompt(nullptr){
    
    if(likelihoodWindows.size() < 2) throw std::runtime_error("The time division must bear at least one off-time window.");
    
    distributions["timeIntervals"] = TH1D("timeIntervals", "#DeltaT distribution", timeDivision.getSpannedNumberOfBins(), 0, timeDivision.getSpannedAnalysisTime());
    distributions["onTimeDistance"] = TH1D("onTimeDistanceDistribution", "on-time distance distribution", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions["onTimeNeutronMultiplicity"] = TH1D("onTimeNeutronMultiplicity", "on-time neutron multiplicity",  neutronMultiplicityBinning.numberOfBins, neutronMultiplicityBinning.lowerEdge, neutronMultiplicityBinning.upperEdge);
    distributions["offTimeDistance"] = TH1D("offTimeDistanceDistribution", "off-time distance distribution", distanceBinning.numberOfBins, distanceBinning.lowerEdge, distanceBinning.upperEdge);
    distributions["offTimeNeutronMultiplicity"] = TH1D("offTimeNeutronMultiplicity", "off-time neutron multiplicity", neutronMultiplicityBinning.numberOfBins, neutronMultiplicityBinning.lowerEdge, neutronMultiplicityBinning.upperEdge);
    
    distributions["onTimeSpectrum"] = TH1D("onTimeSpectrum", "onTimeSpectrum", energyBinning.numberOfBins, energyBinning.lowerEdge, energyBinning.upperEdge);
    distributions["offTimeSpectrum"] = TH1D("offTimeSpectrum", "offTimeSpectrum", energyBinning.numberOfBins, energyBinning.lowerEdge, energyBinning.upperEdge);
    
    for(auto& histPair : distributions) histPair.second.Sumw2();
    
    distributions.at("timeIntervals").GetXaxis()->SetTitle("time to previous #mu (ms)");
    distributions.at("timeIntervals").GetXaxis()->SetTitleOffset(1.2);
    std::string yTitle = "Entries (/"+std::to_string(static_cast<unsigned>(distributions.at("timeIntervals").GetBinWidth(1)))+"ms)";
    distributions.at("timeIntervals").GetYaxis()->SetTitle(yTitle.c_str());
    
  }
  
  template <class T>
  double CandidateMuonPairAnalyser<T>::getOnTimeDuration() const{
    
    return likelihoodWindows.front().getLength();
    
  }
  
  template <class T>
  double CandidateMuonPairAnalyser<T>::getOffTimeDuration() const{
    
    return likelihoodWindows.back().getEndTime() - (likelihoodWindows.begin()+1)->getStartTime();
    
  }
  
  
  template <class T>
  double CandidateMuonPairAnalyser<T>::getSpannedAnalysisDuration() const{
    
    return likelihoodWindows.back().getEndTime() - likelihoodWindows.front().getStartTime();
    
  }
  
  template <class T>
  double CandidateMuonPairAnalyser<T>::getLikeLihoodCut() const{
   
    return likelihoodCut;
    
  }
  
  template <class T>
  const TH1D& CandidateMuonPairAnalyser<T>::getDistribution(const std::string& distributionName) const{
    
    return distributions.at(distributionName);
    
  }
  
  template <class T>
  const std::unordered_map<std::string, TH1D>& CandidateMuonPairAnalyser<T>::getDistributions() const{
    
    return distributions;
    
  }
  
  template <class T>
  std::unordered_map<std::string, TH1D> CandidateMuonPairAnalyser<T>::getBackgroundSubtractedDistributions() const{
    
    std::unordered_map<std::string, TH1D> backgroundSubtractedDistributions;
    
    backgroundSubtractedDistributions["backgroundSubtractedDistance"] = distributions.at("onTimeDistance") - getOnTimeDuration()/getOffTimeDuration() * distributions.at("offTimeDistance");
    backgroundSubtractedDistributions["backgroundSubtractedNeutronMultiplicity"] = distributions.at("onTimeNeutronMultiplicity") - getOnTimeDuration()/getOffTimeDuration() * distributions.at("offTimeNeutronMultiplicity");
    backgroundSubtractedDistributions["backgroundSubtractedSpectrum"] = distributions.at("onTimeSpectrum") - getOnTimeDuration()/getOffTimeDuration() * distributions.at("offTimeSpectrum");
    
    return backgroundSubtractedDistributions;
    
  }
    
  template <class T>
  void CandidateMuonPairAnalyser<T>::setLikeLihoodCut(double likelihoodCut){
    
    this->likelihoodCut = likelihoodCut;
    resetDistributions();
    
  }
  
  template <class T>
  void CandidateMuonPairAnalyser<T>::setTimeDivision(const TimeDivision& timeDivision){
    
    likelihoodWindows = timeDivision.getEqualLengthWindows<MaxWindow<double>>();
    if(likelihoodWindows.size() < 2) throw std::runtime_error("The time division must bear at least one off-time window.");
    resetDistributions();
    
  }
  
  template <class T>
  void CandidateMuonPairAnalyser<T>::setPrompt(const CosmogenicHunter::Single<T>& prompt){
    
    for(auto& window : likelihoodWindows) window.resetValue();
    this->prompt = &prompt;

  }
  
  template <class T>
  template <class K>
  void CandidateMuonPairAnalyser<T>::analyse(const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower){
    
    auto timeInterval = Converter::nanosecondsToMilliseconds(CosmogenicHunter::getTimeCorrelation(*prompt, muonShower.getInitiator()));
    distributions["timeIntervals"].Fill(timeInterval);
    
    auto itWindow = std::find_if(likelihoodWindows.begin(), likelihoodWindows.end(), [&](const auto& window){return window.covers(timeInterval);});
    if(itWindow != likelihoodWindows.end()){
      
      itWindow->updateValue(likelihoodComputer.getLikelihood(*prompt, muonShower));
      auto distance = CosmogenicHunter::getDistanceBetween(*prompt, muonShower.getInitiator());
      
      if(itWindow == likelihoodWindows.begin()){
	
	distributions["onTimeDistance"].Fill(distance);
	distributions["onTimeNeutronMultiplicity"].Fill(muonShower.getNumberOfFollowers());
	
      }
      else{
	
	distributions["offTimeDistance"].Fill(distance);
	distributions["offTimeNeutronMultiplicity"].Fill(muonShower.getNumberOfFollowers());
	
	
      }
      
    }
    
  }
  
  template <class T>
  void CandidateMuonPairAnalyser<T>::updateDistributions(){
    
    updateSpectra();
    
  }
    
  template <class T>
  void CandidateMuonPairAnalyser<T>::resetDistributions(){
    
    for(auto& histPair : distributions) histPair.second.Reset();
    prompt = nullptr;
    
  }
  
  template <class T>
  TimeIntervalsFitResults CandidateMuonPairAnalyser<T>::fitTimeIntervals(double lifetime, double numberOfNeutrinos, double muonRate){
    
    if(distributions.at("timeIntervals").GetNbinsX() > 0){
    
      muonRate *= Converter::millisecondsToSeconds(1);// convert s^-1 into ms^-1
      auto timebinWidth = distributions.at("timeIntervals").GetBinWidth(1);
      
      TF1 fitFunction("fitFunction", [&](double* x, double* p){
	return timebinWidth * (p[0]/lifetime*std::exp(-x[0]/lifetime) + p[1]*muonRate-std::pow(Converter::millisecondsToSeconds(p[2]),2)*x[0]);
      }, 0, getSpannedAnalysisDuration(), 3);
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
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const CandidateMuonPairAnalyser<T>& candidateMuonPairAnalyser){
    
    output<<"Likelihood cut: "<<candidateMuonPairAnalyser.getLikeLihoodCut();    
    return output;
    
  }

}
  
#endif
#ifndef COSMOGENIC_ANALYSER_CANDIDATE_MUON_PAIR_ANALYSER_H
#define COSMOGENIC_ANALYSER_CANDIDATE_MUON_PAIR_ANALYSER_H

#include <unordered_map>
#include "TH1D.h"
#include "Cosmogenic/Shower.hpp"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Single.hpp"
#include "Converter.hpp"
#include "TimeDivision.hpp"
#include "Binning.hpp"
#include "TimeIntervalsFitResults.hpp"

namespace CosmogenicAnalyser{
  
  class CandidateMuonPairAnalyser{
    
    TimeDivision timeDivision;
    std::unordered_map<std::string, TH1D> distributions;
    
  public:  
    CandidateMuonPairAnalyser(TimeDivision timeDivision, const Binning& distanceBinning, const Binning& neutronMultiplicityBinning);
    const TimeDivision& getTimeDivision() const;
    const TH1D& getDistribution(const std::string& distributionName) const;
    const std::unordered_map<std::string, TH1D>& getDistributions() const;
    std::unordered_map<std::string, TH1D> getBackgroundSubtractedDistributions() const;
    void setTimeDivision(TimeDivision timeDivision);
    void reset();
    template <class T, class K>
    void analyse(const CosmogenicHunter::Single<T>& prompt, const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower);
    TimeIntervalsFitResults fitTimeIntervals(double lifetime, double numberOfNeutrinos, double muonRate);
    
  };
  
  template <class T, class K>
  void CandidateMuonPairAnalyser::analyse(const CosmogenicHunter::Single<T>& prompt, const CosmogenicHunter::Shower<CosmogenicHunter::Muon<K>, CosmogenicHunter::Single<T>>& muonShower){
    
    auto distance = CosmogenicHunter::getDistanceBetween(prompt, muonShower.getInitiator());
    auto timeIntervalValue = Converter::nanosecondsToMilliseconds(CosmogenicHunter::getTimeCorrelation(prompt, muonShower.getInitiator()));
    distributions["timeIntervals"].Fill(timeIntervalValue);
    
    if(timeDivision.getOnTimeWindow().contains(timeIntervalValue)){
      
      distributions["onTimeDistance"].Fill(distance);
      distributions["onTimeNeutronMultiplicity"].Fill(muonShower.getNumberOfFollowers());
      
    }
    else if(timeDivision.getOffTimeWindow().contains(timeIntervalValue)){
      
      distributions["offTimeDistance"].Fill(distance);
      distributions["offTimeNeutronMultiplicity"].Fill(muonShower.getNumberOfFollowers());
      
    }
    
  }

}
  
#endif
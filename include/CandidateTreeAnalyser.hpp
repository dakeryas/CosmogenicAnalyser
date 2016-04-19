#ifndef COSMOGENIC_ANALYSER_CANDIDATE_TREE_ANALYSER_H
#define COSMOGENIC_ANALYSER_CANDIDATE_TREE_ANALYSER_H

#include "Cosmogenic/CandidateTree.hpp"
#include "PairSelector.hpp"
#include "MuonShowerSelector.hpp"
#include "CandidateMuonPairAnalyser.hpp"
#include "MuonShowerAnalyser.hpp"

namespace CosmogenicAnalyser{
  
  template <class T, class K>
  class CandidateTreeAnalyser{
    
    PairSelector<T> pairSelector;
    MuonShowerSelector<K> muonShowerSelector;
    
    CandidateMuonPairAnalyser<T> candidateMuonPairAnalyser;
    MuonShowerAnalyser muonShowerAnalyser;
    TH1F muonSpectrum;
    double liveTime;
    
  public:  
    CandidateTreeAnalyser(PairSelector<T> pairSelector, MuonShowerSelector<K> muonShowerSelector, CandidateMuonPairAnalyser<T> candidateMuonPairAnalyser, MuonShowerAnalyser muonShowerAnalyser);
    void analyse(const CosmogenicHunter::CandidateTree<T,K>& candidateTree);
    template<class CandidateTreeIterator>
    void analyse(CandidateTreeIterator begin, CandidateTreeIterator end);
    void resetDistributions();
    const std::vector<TH1D>& getCandidateMuonPairDistributions() const;
    std::vector<TH1D> getBackgroundSubtractedCandidateMuonPairDistributions() const;
    const std::vector<TH1F>& getNeutronDistributions() const;
    const TH1F& getMuonSpectrum() const;
    double getLiveTime() const;
    double getMuonRate() const;
    double getNumberOfCandidates() const;
    TimeIntervalsFitResults fitTimeIntervals(double cosmogenicLifetime);
    
  };
  
  template <class T, class K>
  CandidateTreeAnalyser<T,K>::CandidateTreeAnalyser(PairSelector<T> pairSelector, MuonShowerSelector<K> muonShowerSelector, CandidateMuonPairAnalyser<T> candidateMuonPairAnalyser, MuonShowerAnalyser muonShowerAnalyser)
  :pairSelector(std::move(pairSelector)),muonShowerSelector(std::move(muonShowerSelector)),candidateMuonPairAnalyser(std::move(candidateMuonPairAnalyser)),muonShowerAnalyser(std::move(muonShowerAnalyser)),
  muonSpectrum("muonSpectrum", "muonSpectrum", 80, 0, 900),
  liveTime(){
    
    muonSpectrum.Sumw2();
    
  }
  
  template <class T, class K>
  void CandidateTreeAnalyser<T,K>::analyse(const CosmogenicHunter::CandidateTree<T,K>& candidateTree){
    
    if(pairSelector.tag(candidateTree.getCandidatePair())){
    
      candidateMuonPairAnalyser.setPrompt(candidateTree.getCandidatePair().getPrompt());
      muonShowerAnalyser.resetIdentifiers();
      
      for(const auto& muonShower : candidateTree.getMuonShowers()){
	
	if(muonShowerSelector.tag(muonShower)){
	  
	  candidateMuonPairAnalyser.analyse(muonShower);
	  muonSpectrum.Fill(muonShowerSelector.getVisibleEnergy(muonShower.getInitiator()));
	  muonShowerAnalyser.analyse(muonShower);
	  
	}
	
      }
      
      candidateMuonPairAnalyser.updateDistributions();
      liveTime += candidateTree.getMuonShowers().getLength();
      
    }
    
  }
  
  template <class T, class K>
  template<class CandidateTreeIterator>
  void CandidateTreeAnalyser<T,K>::analyse(CandidateTreeIterator begin, CandidateTreeIterator end){
    
    std::for_each(begin, end, [this](const auto& candidateTree){this->analyse(candidateTree);});
    
  }
  
  template <class T, class K>
  void CandidateTreeAnalyser<T,K>::resetDistributions(){
    
    candidateMuonPairAnalyser.resetDistributions();
    muonShowerAnalyser.resetDistributions();
    muonSpectrum.Reset();
    liveTime = 0;
    
  }
  
  template <class T, class K>
  const std::vector<TH1D>& CandidateTreeAnalyser<T,K>::getCandidateMuonPairDistributions() const{
    
    return candidateMuonPairAnalyser.getDistributions();
    
  }
  
  template <class T, class K>
  const std::vector<TH1F>& CandidateTreeAnalyser<T,K>::getNeutronDistributions() const{
    
    return muonShowerAnalyser.getDistributions();
    
  }
  
  template <class T, class K>
  std::vector<TH1D> CandidateTreeAnalyser<T,K>::getBackgroundSubtractedCandidateMuonPairDistributions() const{
    
    return candidateMuonPairAnalyser.getBackgroundSubtractedDistributions();
    
  }
  
  template <class T, class K>
  const TH1F& CandidateTreeAnalyser<T,K>::getMuonSpectrum() const{
    
    return muonSpectrum;
    
  }
  
  template <class T, class K>
  double CandidateTreeAnalyser<T,K>::getLiveTime() const{
    
    return liveTime;
    
  }
  
  template <class T, class K>
  double CandidateTreeAnalyser<T,K>::getMuonRate() const{
    
    return muonSpectrum.GetEntries()/Converter::nanosecondsToSeconds(liveTime);
    
  }
  
  template <class T, class K>
  double CandidateTreeAnalyser<T,K>::getNumberOfCandidates() const{
    
    return Converter::nanosecondsToSeconds(liveTime) / Converter::millisecondsToSeconds(candidateMuonPairAnalyser.getSpannedAnalysisDuration());
    
  }
  
  template <class T, class K>
  TimeIntervalsFitResults CandidateTreeAnalyser<T,K>::fitTimeIntervals(double cosmogenicLifetime){
    
    return candidateMuonPairAnalyser.fitTimeIntervals(cosmogenicLifetime, getNumberOfCandidates(), getMuonRate());
    
  }
  
  template <class T, class K>
  std::ostream& operator<<(std::ostream& output, const CandidateTreeAnalyser<T,K>& candidateTreeAnalyser){
    
    TablePrinter tablePrinter(output, '-', "|", 1, "Live time (s)", "Number of muons", "Muon rate (Hz)", "Number of candidates");
    tablePrinter.setPrecision(0, 0, 2, 0);
    tablePrinter.print(
      Converter::nanosecondsToSeconds(candidateTreeAnalyser.getLiveTime()),
      candidateTreeAnalyser.getMuonSpectrum().GetEntries(),
      candidateTreeAnalyser.getMuonRate(),
      candidateTreeAnalyser.getNumberOfCandidates()
    );
     
    return output;
    
  }

}
  
#endif
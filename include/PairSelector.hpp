#ifndef COSMOGENIC_ANALYSER_PAIR_SELECTOR_H
#define COSMOGENIC_ANALYSER_PAIR_SELECTOR_H

#include "Cosmogenic/CandidatePair.hpp"
#include "Cosmogenic/Veto.hpp"

namespace CosmogenicAnalyser{

  template <class T>//to accept entries saved with the accuracy of type T
  class PairSelector{
    
    CosmogenicHunter::Bounds<T> promptEnergyBounds;
    std::vector<std::unique_ptr<CosmogenicHunter::Veto<T>>> vetoes;
    
  public:
    PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds);
    PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, std::vector<std::unique_ptr<CosmogenicHunter::Veto<T>>> vetoes);
    PairSelector(const PairSelector<T>& other);
    PairSelector(PairSelector&& other) = default;
    PairSelector<T>& operator = (PairSelector<T> other);
    PairSelector<T>& operator = (PairSelector<T>&& other) = default;
    const std::vector<std::unique_ptr<CosmogenicHunter::Veto<T>>>& getVetoes() const;
    template <class VetoType, class... Args>
    void emplaceVeto(Args&&... args);
    void removeVeto(const std::string& vetoName);
    void removeVetoes();
    bool tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const;
    
  };
  
  template <class T>
  PairSelector<T>::PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds)
  :promptEnergyBounds(std::move(promptEnergyBounds)){
  
  }
  
  template <class T>
  PairSelector<T>::PairSelector(CosmogenicHunter::Bounds<T> promptEnergyBounds, std::vector<std::unique_ptr<CosmogenicHunter::Veto<T>>> vetoes)
  :promptEnergyBounds(std::move(promptEnergyBounds)),vetoes(std::move(vetoes)){
  
  }
  
  template <class T>
  PairSelector<T>::PairSelector(const PairSelector<T>& other){
    
    promptEnergyBounds = other.promptEnergyBounds;
    for(const auto& vetoPtr : other.vetoes) vetoes.emplace_back(vetoPtr->clone());
    
  }

  template <class T>
  PairSelector<T>& PairSelector<T>::operator = (PairSelector<T> other){
    
    std::swap(promptEnergyBounds, other.promptEnergyBounds);
    std::swap(vetoes , other.vetoes);
    return *this;
    
  }
  
  template <class T>
  const std::vector<std::unique_ptr<CosmogenicHunter::Veto<T>>>& PairSelector<T>::getVetoes() const{
    
    return vetoes;

  }
  
  template <class T>
  template <class VetoType, class... Args>
  void PairSelector<T>::emplaceVeto(Args&&... args){
    
    vetoes.emplace_back(std::make_unique<VetoType>(std::forward<Args>(args)...));

  }
  
  template <class T>
  void PairSelector<T>::removeVeto(const std::string& vetoName){
    
    vetoes.erase(std::remove_if(vetoes.begin(), vetoes.end(),[&](const auto& veto){return veto.getName() == vetoName;}), vetoes.end());
    
  }
  
  template <class T>
  void PairSelector<T>::removeVetoes(){
    
    vetoes.clear();

  }
  
  template <class T>
  bool PairSelector<T>::tag(const CosmogenicHunter::CandidatePair<T>& candidatePair) const{
    
    if(candidatePair.getPrompt().hasVisibleEnergyWithin(promptEnergyBounds)){
      
      for(const auto& vetoPtr : vetoes) if(vetoPtr->veto(candidatePair)) return false;//one veto is enough to discard the event
      return true;//means that it was not vetoed by any veto
      
    }
    else return false;
    
  }
  
}

#endif
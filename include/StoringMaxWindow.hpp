#ifndef COSMOGENIC_ANALYSER_STORING_MAX_WINDOW_H
#define COSMOGENIC_ANALYSER_STORING_MAX_WINDOW_H

#include "MaxWindow.hpp"

namespace CosmogenicAnalyser{

  template <class T, class K>
  class StoringMaxWindow : public MaxWindow<T>{
    
    K storedItem; 
    
  public:
    using MaxWindow<T>::MaxWindow;
    void store(K storedItem);
    K getStoredItem() const;
    
  };
  
  template <class T, class K>
  void StoringMaxWindow<T,K>::store(K storedItem){
    
    this->storedItem = std::move(storedItem);
    
  }
  
  template <class T, class K>
  K StoringMaxWindow<T,K>::getStoredItem() const{
    
    return storedItem;
    
  }

}

#endif
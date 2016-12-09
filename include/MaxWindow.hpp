#ifndef COSMOGENIC_ANALYSER_MAX_WINDOW_H
#define COSMOGENIC_ANALYSER_MAX_WINDOW_H

#include "ValuedWindow.hpp"

namespace CosmogenicAnalyser{

  template <class T>
  class MaxWindow : public ValuedWindow<T>{
    
  public:
    using ValuedWindow<T>::ValuedWindow;
    bool updateValue(const T& value);
    
  };
  
  template <class T>
  bool MaxWindow<T>::updateValue(const T& value){
    
    bool update = value > this->value;
    if(update) this->value = value;
    return update;
    
  }

}

#endif
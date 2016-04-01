#ifndef COSMOGENIC_ANALYSER_MAX_WINDOW_H
#define COSMOGENIC_ANALYSER_MAX_WINDOW_H

#include "ValuedWindow.hpp"

namespace CosmogenicAnalyser{

  template <class T>
  class MaxWindow : public ValuedWindow<T>{
    
  public:
    MaxWindow(double startTime, double endTime);
    MaxWindow(double startTime, double endTime, T minValue);
    MaxWindow(TimeWindow timeWindow);
    MaxWindow(TimeWindow timeWindow, T minValue);
    void updateValue(const T& value);
    
  };
  
  template <class T>
  MaxWindow<T>::MaxWindow(double startTime, double endTime):ValuedWindow<T>(startTime, endTime){
    
  }
  
  template <class T>
  MaxWindow<T>::MaxWindow(double startTime, double endTime, T minValue):ValuedWindow<T>(startTime, endTime, std::move(minValue)){
    
  }
  
  template <class T>
  MaxWindow<T>::MaxWindow(TimeWindow timeWindow):ValuedWindow<T>(std::move(timeWindow)){
    
  }
  
  template <class T>
  MaxWindow<T>::MaxWindow(TimeWindow timeWindow, T minValue):ValuedWindow<T>(std::move(timeWindow), std::move(minValue)){
    
  }
  
  
  template <class T>
  void MaxWindow<T>::updateValue(const T& value){
    
    if(value > this->value) this->value = value;
    
  }

}

#endif
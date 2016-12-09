#ifndef COSMOGENIC_ANALYSER_VALUED_WINDOW_H
#define COSMOGENIC_ANALYSER_VALUED_WINDOW_H

#include "TimeWindow.hpp"

namespace CosmogenicAnalyser{

  template <class T>
  class ValuedWindow : public TimeWindow{
    
  protected:
    T value;
    
  public:
    ValuedWindow(double startTime, double endTime);
    ValuedWindow(double startTime, double endTime, T value);
    ValuedWindow(TimeWindow timeWindow);
    ValuedWindow(TimeWindow timeWindow, T value);
    virtual const T& getValue() const;
    virtual bool updateValue(const T& value) = 0;
    virtual void resetValue();//reset value to T{}
    virtual void print(std::ostream& output) const;
    
  };
  
  template <class T>
  ValuedWindow<T>::ValuedWindow(double startTime, double endTime):TimeWindow(startTime, endTime),value(){
    
  }
  
  template <class T>
  ValuedWindow<T>::ValuedWindow(double startTime, double endTime, T value):TimeWindow(startTime, endTime),value(std::move(value)){
    
  }
  
  template <class T>
  ValuedWindow<T>::ValuedWindow(TimeWindow timeWindow):TimeWindow(std::move(timeWindow)),value(){
    
  }
  
  template <class T>
  ValuedWindow<T>::ValuedWindow(TimeWindow timeWindow, T value):TimeWindow(std::move(timeWindow)),value(std::move(value)){
    
  }
  
  template <class T>
  const T& ValuedWindow<T>::getValue() const{
    
    return value;
    
  }
  
  template <class T>
  void ValuedWindow<T>::resetValue(){
    
    value = T{};
    
  }
  
  template <class T>
  void ValuedWindow<T>::print(std::ostream& output) const{
    
    TimeWindow::print(output);
    output<<" --> "<<value;
    
  }

  template <class T>
  std::ostream& operator<<(std::ostream& output, const ValuedWindow<T>& valuedWindow){
    
    valuedWindow.print(output);
    return output;
    
  }

}

#endif
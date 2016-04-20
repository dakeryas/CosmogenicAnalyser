#ifndef COSMOGENIC_ANALYSER_ROOT_OBJECT_EXTRACTOR_H
#define COSMOGENIC_ANALYSER_ROOT_OBJECT_EXTRACTOR_H

#include "TFile.h"
#include <stdexcept>

namespace CosmogenicAnalyser{
  
  template <class T>
  class RootObjectExtractor{
    
  public:
    static T extract(const std::string& keyName, TFile& file);
    
  };
  
  template <class T>
  T RootObjectExtractor<T>::extract(const std::string& keyName, TFile& file){
    
    auto object = dynamic_cast<T*>(file.Get(keyName.c_str()));
    if(object != nullptr) return *object;
    else throw std::runtime_error("Could not extact '"+keyName+"' from "+file.GetName());
    
  }
  
  template <class T>
  class RootObjectExtractor<T*>{
    
  public:
    static T* extract(const std::string& keyName, TFile& file);
    
  };
  
  template <class T>
  T* RootObjectExtractor<T*>::extract(const std::string& keyName, TFile& file){
    
    auto object = dynamic_cast<T*>(file.Get(keyName.c_str()));
    if(object != nullptr) return object;
    else throw std::runtime_error("Could not extact '"+keyName+"' from "+file.GetName());
    
  }
  
}

#endif
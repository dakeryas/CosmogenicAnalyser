#ifndef COSMOGENIC_ANALYSER_CONVERTER_H
#define COSMOGENIC_ANALYSER_CONVERTER_H

namespace CosmogenicAnalyser{
  
  namespace Converter{
    
    template <class T>
    double nanosecondsToMicroseconds(T nanoseconds){
      
      return nanoseconds * 1e-3;
      
    }
    
    template <class T>
    double nanosecondsToMilliseconds(T nanoseconds){
      
      return nanoseconds * 1e-6;
      
    }
    
    template <class T>
    double nanosecondsToSeconds(T nanoseconds){
      
      return nanoseconds * 1e-9;
      
    }
    
    template <class T>
    double millisecondsToSeconds(T milliseconds){
      
      return milliseconds * 1e-3;
      
    }
    
  }
    
}
  
#endif
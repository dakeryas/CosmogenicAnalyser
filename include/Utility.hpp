#ifndef COSMOGENIC_ANALYSER_UTILITY_H
#define COSMOGENIC_ANALYSER_UTILITY_H

#include <iomanip>

namespace CosmogenicAnalyser{

  namespace Utility{
    
    std::string toString(const char* characterChain);//avoid processing for const char*
    std::string toString(const std::string& string);//avoid processing for const std::string&
    
    template <class T>
    std::string toString(T&& object, int precision = -1) {
      
      std::ostringstream output;
      if(precision > -1) output<<std::fixed<<std::setprecision(precision)<<object;
      else output<<object;
      return output.str();
      
    }
    
  }

}

#endif
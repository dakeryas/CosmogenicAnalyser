#ifndef COSMOGENIC_ANALYSER_UTILITY_H
#define COSMOGENIC_ANALYSER_UTILITY_H

#include <iomanip>
#include <algorithm>
#include <stdexcept>

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
    
    template <class DistributionIterator>
    auto& getDistribution(const std::string& distributionName, DistributionIterator distributionBegin, DistributionIterator distributionEnd){
      
      auto it = std::find_if(distributionBegin, distributionEnd, [&](const auto& distribution){return distribution.GetName() == distributionName;});
      if(it != distributionEnd) return *it;
      else throw std::runtime_error(distributionName+" cannot be found.");
      
    }
    
    template <class DistributionContainer>
    auto& getDistribution(const std::string& distributionName, const DistributionContainer& distributions){
      
      return getDistribution(distributionName, std::begin(distributions), std::end(distributions));
      
    }
    
  }

}

#endif
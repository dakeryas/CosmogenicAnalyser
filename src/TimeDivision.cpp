#include "TimeDivision.hpp"

namespace CosmogenicAnalyser{
  
  TimeDivision::TimeDivision(double timebinWidth, double onTimeWindowLength, double offTimeWindowLength)
  :timebinWidth(timebinWidth),onTimeWindowLength(onTimeWindowLength),offTimeWindowLength(offTimeWindowLength){
    
    if(timebinWidth <  0 || onTimeWindowLength < 0 || offTimeWindowLength < 0) throw std::invalid_argument("Durations for time division cannot be negative.");
    else if(4 * timebinWidth > onTimeWindowLength + offTimeWindowLength) throw std::invalid_argument("The time bin width must be smaller than a fourth of the analysis time.");
    
  }
  
  double TimeDivision::getTimeBinWidth() const{
    
    return timebinWidth;

  }

  double TimeDivision::getOnTimeWindowLength() const{
    
    return onTimeWindowLength;

  }

  double TimeDivision::getOffTimeWindowLength() const{

    return offTimeWindowLength;
    
  }

  double TimeDivision::getAnalysisTime() const{

    return onTimeWindowLength + offTimeWindowLength;
    
  }

  unsigned TimeDivision::getNumberOfBins() const{

    return getAnalysisTime() / timebinWidth;
    
  }
  
  std::ostream& operator<<(std::ostream& output, const TimeDivision& timeDivision){
    
    output<<std::setw(12)<<std::left<<"Bin width: "<<std::setw(5)<<std::left<<timeDivision.getTimeBinWidth()<<"\n"
      <<std::setw(12)<<std::left<<"On-time: "<<std::setw(7)<<std::right<<timeDivision.getOnTimeWindowLength()<<"\n"
      <<std::setw(12)<<std::left<<"Off-time: "<<std::setw(7)<<std::right<<timeDivision.getOffTimeWindowLength();
      
    return output;
    
  }
  
}

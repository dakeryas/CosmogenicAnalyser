#include "TimeDivision.hpp"

namespace CosmogenicAnalyser{
  
  TimeDivision::TimeDivision(double timeBinWidth, TimeWindow onTimeWindow, TimeWindow offTimeWindow)
  :timeBinWidth(timeBinWidth),onTimeWindow(std::move(onTimeWindow)),offTimeWindow(std::move(offTimeWindow)){
    
    if(timeBinWidth <  0) throw std::invalid_argument(std::to_string(timeBinWidth)+"is not valid time bin width");
    else if(getAnalysisGapLength() < 0) throw std::invalid_argument("On-time and off-time windows should not overalp.");
    else if(4 * timeBinWidth > getAnalysisTime()) throw std::invalid_argument("The time bin width must be smaller than a fourth of the analysis time.");
    
  }
  
  double TimeDivision::getTimeBinWidth() const{
    
    return timeBinWidth;

  }
  
  const TimeWindow& TimeDivision::getOnTimeWindow() const{
    
    return onTimeWindow;

  }

  const TimeWindow& TimeDivision::getOffTimeWindow() const{

    return offTimeWindow;
    
  }

  double TimeDivision::getOnTimeWindowLength() const{
    
    return onTimeWindow.getLength();

  }

  double TimeDivision::getOffTimeWindowLength() const{

    return offTimeWindow.getLength();
    
  }

  double TimeDivision::getAnalysisTime() const{

    return getOnTimeWindowLength() + getOffTimeWindowLength();
    
  }
  
  double TimeDivision::getSpannedAnalysisTime() const{
    
    return offTimeWindow.getEndTime() - onTimeWindow.getStartTime();

  }
  
  double TimeDivision::getAnalysisGapLength() const{
    
    return offTimeWindow.getStartTime() - onTimeWindow.getEndTime();

  }

  unsigned TimeDivision::getNumberOfBins() const{

    return std::round(getAnalysisTime() / timeBinWidth);
    
  }
  
  unsigned TimeDivision::getSpannedNumberOfBins() const{

    return std::round(getSpannedAnalysisTime() / timeBinWidth);
    
  }
  
  double TimeDivision::getWindowsLengthsRatio() const{
    
    return getOffTimeWindowLength() / getOnTimeWindowLength();

  }

  std::ostream& operator<<(std::ostream& output, const TimeDivision& timeDivision){
    
    output<<std::setw(12)<<std::left<<"Bin width: "<<std::setw(5)<<std::left<<timeDivision.getTimeBinWidth()<<"\n"
      <<std::setw(12)<<std::left<<"On-time: "<<std::setw(7)<<std::right<<timeDivision.getOnTimeWindowLength()<<"\n"
      <<std::setw(12)<<std::left<<"Off-time: "<<std::setw(7)<<std::right<<timeDivision.getOffTimeWindowLength();
      
    return output;
    
  }
  
}

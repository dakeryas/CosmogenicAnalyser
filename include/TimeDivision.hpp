#ifndef COSMOGENIC_ANALYSER_TIME_DIVISION_H
#define COSMOGENIC_ANALYSER_TIME_DIVISION_H

#include "Cosmogenic/Bounds.hpp"

namespace CosmogenicAnalyser{
  
  class TimeDivision{
    
    using TimeWindow = CosmogenicHunter::Bounds<double>;
    
    double timeBinWidth;
    TimeWindow onTimeWindow;
    TimeWindow offTimeWindow;
    
    
  public:
    TimeDivision(double timeBinWidth, TimeWindow onTimeWindow, TimeWindow offTimeWindow);
    double getTimeBinWidth() const;
    TimeWindow getOnTimeWindow() const;
    TimeWindow getOffTimeWindow() const;
    double getOnTimeWindowLength() const;
    double getOffTimeWindowLength() const;
    double getAnalysisTime() const;
    double getAnalysisGapLength() const;//time gap between the on and off windows
    double getSpannedAnalysisTime() const;
    unsigned getNumberOfBins() const;
    unsigned getSpannedNumberOfBins() const;
    double getWindowsLenghtsRatio() const;
    
  };
  
  std::ostream& operator<<(std::ostream& output, const TimeDivision& TimeDivision);
  
}
  
#endif
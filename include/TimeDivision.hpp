#ifndef COSMOGENIC_ANALYSER_TIME_DIVISION_H
#define COSMOGENIC_ANALYSER_TIME_DIVISION_H

#include <stdexcept>
#include <iomanip>

namespace CosmogenicAnalyser{
  
  class TimeDivision{
    
    double timebinWidth;
    double onTimeWindowLength;
    double offTimeWindowLength;
    
  public:
    TimeDivision(double timebinWidth, double onTimeWindowLength, double offTimeWindowLength);
    double getTimeBinWidth() const;
    double getOnTimeWindowLength() const;
    double getOffTimeWindowLength() const;
    double getAnalysisTime() const;
    unsigned getNumberOfBins() const;
    
  };
  
  std::ostream& operator<<(std::ostream& output, const TimeDivision& TimeDivision);
  
}
  
#endif
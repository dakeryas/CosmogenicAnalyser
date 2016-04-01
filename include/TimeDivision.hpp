#ifndef COSMOGENIC_ANALYSER_TIME_DIVISION_H
#define COSMOGENIC_ANALYSER_TIME_DIVISION_H

#include "TimeWindow.hpp"

namespace CosmogenicAnalyser{
  
  class TimeDivision{
    
    double timeBinWidth;
    TimeWindow onTimeWindow;
    TimeWindow offTimeWindow;
    
    
  public:
    TimeDivision(double timeBinWidth, TimeWindow onTimeWindow, TimeWindow offTimeWindow);
    double getTimeBinWidth() const;
    const TimeWindow& getOnTimeWindow() const;
    const TimeWindow& getOffTimeWindow() const;
    template <class T = TimeWindow>//classes derived from TimeDivision can also be used
    std::vector<T> getEqualLengthOffTimeWindows() const;
    template <class T = TimeWindow>//classes derived from TimeDivision can also be used
    std::vector<T> getEqualLengthWindows() const;//on-time + splits the offTimeWindow into a vector of windows whose lengths are all equal to the on-time's
    double getOnTimeWindowLength() const;
    double getOffTimeWindowLength() const;
    double getAnalysisTime() const;
    double getAnalysisGapLength() const;//time gap between the on and off windows
    double getSpannedAnalysisTime() const;
    unsigned getNumberOfBins() const;
    unsigned getSpannedNumberOfBins() const;
    double getWindowsLengthsRatio() const;
    
  };
  
  std::ostream& operator<<(std::ostream& output, const TimeDivision& TimeDivision);
  
  template <class T>
  std::vector<T> TimeDivision::getEqualLengthOffTimeWindows() const{
    
    std::vector<T> windows;
    int ratio = getWindowsLengthsRatio();
    for(int k = 0; k < ratio; ++k) windows.emplace_back(offTimeWindow.getStartTime() + k * getOnTimeWindowLength(), offTimeWindow.getStartTime() + (k+1) * getOnTimeWindowLength());
    return windows;
    
  }
  
  template <class T>
  std::vector<T> TimeDivision::getEqualLengthWindows() const{
    
    std::vector<T> windows;
    windows.emplace_back(getOnTimeWindow());
    auto offTimeWindows = getEqualLengthOffTimeWindows<T>();
    windows.insert(windows.end(), offTimeWindows.begin(), offTimeWindows.end());
    return windows;
    
  }

}
  
#endif
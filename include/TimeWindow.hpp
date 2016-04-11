#ifndef COSMOGENIC_ANALYSER_TIME_WINDOW_H
#define COSMOGENIC_ANALYSER_TIME_WINDOW_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicAnalyser{

  class TimeWindow{
    
    double startTime, endTime;
    
  public:
    TimeWindow() = default;
    TimeWindow(double startTime, double endTime);
    TimeWindow(const TimeWindow& other) = default;
    TimeWindow(TimeWindow&& other) = default;
    TimeWindow& operator = (const TimeWindow& other) = default;
    TimeWindow& operator = (TimeWindow&& other) = default;
    virtual ~TimeWindow() = default;
    double getStartTime() const;
    double getEndTime() const;
    double getLength() const;
    void setStartEndTimes(double startTime, double endTime);
    bool covers(double time) const;//check if the time is within the time window (accept events in [startTime, endTime[ )
    virtual void print(std::ostream& output) const;
    
  };

  std::ostream& operator<<(std::ostream& output, const TimeWindow& window);
  std::istream& operator>>(std::istream& input, TimeWindow& window);

}

#endif
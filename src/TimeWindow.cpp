#include "TimeWindow.hpp"

namespace CosmogenicAnalyser{

  TimeWindow::TimeWindow(double startTime, double endTime):timeBounds(startTime, endTime){
    
  }

  double TimeWindow::getStartTime() const{
    
    return timeBounds.getLowEdge();

  }
  
  double TimeWindow::getEndTime() const{
    
    return timeBounds.getUpEdge();

  }

  double TimeWindow::getLength() const{
    
    return timeBounds.getWidth();

  }

  bool TimeWindow::covers(double time) const{

    return timeBounds.contains(time);

  }
  
  void TimeWindow::print(std::ostream& output) const{
    
    output<<"["<<std::setw(5)<<std::internal<<timeBounds.getLowEdge()<<", "<<std::setw(5)<<std::internal<<timeBounds.getUpEdge()<<"]";
    
  }

  std::ostream& operator<<(std::ostream& output, const TimeWindow& window){
    
    window.print(output);
    return output;
    
  }

}
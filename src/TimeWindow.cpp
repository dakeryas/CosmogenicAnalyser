#include "TimeWindow.hpp"

namespace CosmogenicAnalyser{

  TimeWindow::TimeWindow(double startTime, double endTime):startTime(startTime),endTime(endTime){
    
    if(startTime < 0 || endTime < 0 || endTime < startTime){
      
      auto errorMessage = std::to_string(startTime)+" and "+std::to_string(endTime)+" are not valid start and end times.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  double TimeWindow::getStartTime() const{
    
    return startTime;

  }
  
  double TimeWindow::getEndTime() const{
    
    return endTime;

  }

  double TimeWindow::getLength() const{
    
    return endTime - startTime;

  }
  
  void TimeWindow::setStartEndTimes(double startTime, double endTime){
    
    if(startTime < 0 || endTime < 0 || endTime < startTime){
      
      auto errorMessage = std::to_string(startTime)+" and "+std::to_string(endTime)+" are not valid start and end times.";
      throw std::invalid_argument(errorMessage);
      
    }
    else{
      
      this->startTime = startTime;
      this->endTime = endTime;
      
    }

  }

  bool TimeWindow::covers(double time) const{

    if(time >= startTime && time < endTime) return true;
    else return false;

  }
  
  void TimeWindow::print(std::ostream& output) const{
    
    output<<"["<<std::setw(5)<<std::internal<<startTime<<", "<<std::setw(5)<<std::internal<<endTime<<"]";
    
  }

  std::ostream& operator<<(std::ostream& output, const TimeWindow& window){
    
    window.print(output);
    return output;
    
  }
  
  std::istream& operator>>(std::istream& input, TimeWindow& window){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("(^"+number+")[:,]("+number+"$)");//start with a number :, seprator and end with another number
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) window.setStartEndTimes(std::stod(regexMatches[1]), std::stod(regexMatches[2]));
    else throw std::invalid_argument(token+" cannot be parsed to build the time window.");
    
    return input;
  
  }

}
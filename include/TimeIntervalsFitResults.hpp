#ifndef COSMOGENIC_ANALYSER_TIME_INTERVALS_FIT_RESULTS_H
#define COSMOGENIC_ANALYSER_TIME_INTERVALS_FIT_RESULTS_H

#include "Variable.hpp"
#include "TablePrinter.hpp"

namespace CosmogenicAnalyser{

  struct ChiSquareOverNdf{
    
    double ChiSquare;
    unsigned Ndf;
    
  };
  
  std::ostream& operator<<(std::ostream& output, const ChiSquareOverNdf& chiSquareOverNdf);
  
  struct TimeIntervalsFitResults{
    
    Variable cosmogenics;
    Variable neutrinos;
    Variable accidentalRate;
    ChiSquareOverNdf chiSquareOverNdf;
    
  };

  std::ostream& operator<<(std::ostream& output, const TimeIntervalsFitResults& timeIntervalsFitResults);
  
}

#endif
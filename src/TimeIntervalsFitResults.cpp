#include "TimeIntervalsFitResults.hpp"

namespace CosmogenicAnalyser{

  std::ostream& operator<<(std::ostream& output, const ChiSquareOverNdf& chiSquareOverNdf){
    
    output<<chiSquareOverNdf.ChiSquare<<"/"<<chiSquareOverNdf.Ndf;
    return output;
    
  }
  
  std::ostream& operator<<(std::ostream& output, const TimeIntervalsFitResults& timeIntervalsFitResults){
    
    TablePrinter tablePrinter(output, '-', "|", 1, "Number of cosmogenics", "Number of neutrinos", "Accidental rate", "Chi2/Ndf");
    tablePrinter.setPrecision(0, 0, 2, 1);
    tablePrinter.print(timeIntervalsFitResults.cosmogenics, timeIntervalsFitResults.neutrinos, timeIntervalsFitResults.accidentalRate, timeIntervalsFitResults.chiSquareOverNdf);
    return output;
    
  }
  
}

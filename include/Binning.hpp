#ifndef COSMOGENIC_ANALYSER_BINNING_H
#define COSMOGENIC_ANALYSER_BINNING_H

#include <iomanip>
#include <iostream>

namespace CosmogenicAnalyser{

  struct Binning{
    
    unsigned numberOfBins;
    double lowerEdge;
    double upperEdge;
    
  };
  
  std::ostream& operator<<(std::ostream& output, const Binning& binning);
  
}

#endif
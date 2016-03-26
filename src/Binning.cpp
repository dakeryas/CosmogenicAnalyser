#include "Binning.hpp"

namespace CosmogenicAnalyser{

  std::ostream& operator<<(std::ostream& output, const Binning& binning){
    
    output<<std::setw(5)<<std::left<<binning.numberOfBins
    <<" [ "<<std::setw(6)<<std::right<<binning.lowerEdge
    <<", "<<std::setw(6)<<std::right<<binning.upperEdge<<" ]";
      
    return output;
    
  }
  
}

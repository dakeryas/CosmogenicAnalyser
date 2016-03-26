#include "TablePrinter.hpp"

namespace CosmogenicAnalyser{
  
  TablePrinter::~TablePrinter(){
    
    output<<line;
    
  }
  
  const std::vector<std::string>& TablePrinter::getLabels() const{
    
    return labels;

  }
  
  unsigned TablePrinter::getNumberOfColumns() const{
    
    return labels.size();

  }
  
  void TablePrinter::print(){
    
    output<<columnSeparator<<"\n";
    labelIterator = labels.cbegin();//when done printing the line, set the labelIterator to the first column
    precisionIterator = precisions.cbegin();

  }
  
}
  
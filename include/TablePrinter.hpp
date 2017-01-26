#ifndef COSMOGENIC_ANALYSER_TABLE_PRINTER_H
#define COSMOGENIC_ANALYSER_TABLE_PRINTER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <numeric>
#include "boost/io/ios_state.hpp"
#include "Utility.hpp"

namespace CosmogenicAnalyser{
  
  class TablePrinter{
    
    std::ostream& output;
    boost::io::ios_base_all_saver streamStateSaver;//save precision and flags (restored on destruction)
    
    std::vector<std::string> labels;
    decltype(labels.cbegin()) labelIterator;
    std::vector<int> precisions;
    decltype(precisions.cbegin()) precisionIterator;
    
    std::string columnSeparator;
    std::string line;
    int minimumColumnWidth;
    
  public:
    template <class... Labels>
    TablePrinter(std::ostream& output, char lineSeparator, std::string columnSeparator, int minimumColumnWidth, Labels&&... labels);
    TablePrinter(const TablePrinter& other) = delete;
    TablePrinter(TablePrinter&& other) = default;
    TablePrinter& operator = (const TablePrinter& other) = delete;
    TablePrinter& operator = (TablePrinter&& other) = default;
    ~TablePrinter();
    const std::vector<std::string>& getLabels() const;
    unsigned getNumberOfColumns() const;
    template <class... Precision>
    void setPrecision(Precision&&... precision);
    void print();
    template <class Arg, class... Other>
    void print(const Arg& arg, Other&&... other);
    
  };
  
  template <class... Labels>
  TablePrinter::TablePrinter(std::ostream& output, char lineSeparator, std::string columnSeparator, int minimumColumnWidth, Labels&&... labels)
  :output(output),streamStateSaver(output),labels({labels...}),labelIterator(this->labels.cbegin()),precisions(),precisionIterator(precisions.cbegin()),columnSeparator(std::move(columnSeparator)),minimumColumnWidth(minimumColumnWidth){
    
    if(minimumColumnWidth < 0) throw std::logic_error("Column width cannot be negative.");
    
    auto totalWidth = std::accumulate(this->labels.begin(), this->labels.end(), 0, [](const auto& accumulatedSize, const auto& label){return accumulatedSize + label.size();});
    line = std::string (totalWidth + (this->labels.size()  + 1)*(this->columnSeparator.size() + 2 + minimumColumnWidth) - 2 - minimumColumnWidth, lineSeparator);//account for the two blank spaces per separator (minus 2 for the missing ones begin / last)
    
    output<<line<<"\n";
    print(labels...);
    output<<line<<"\n";

  }
  
  template <class... Precision>
  void TablePrinter::setPrecision(Precision&&... precision){
    
    precisions = {std::forward<Precision>(precision)...};
    precisionIterator = precisions.cbegin();
    
  }
  
  template <class Arg, class... Other>
  void TablePrinter::print(const Arg& arg, Other&&... other){
    
    if(labelIterator != labels.cend()){
      
      if(precisionIterator != precisions.cend()){
	
	if(*precisionIterator > -1) output<<columnSeparator<<" "<<std::setw(labelIterator->size() + minimumColumnWidth)<<Utility::toString(arg, *precisionIterator)<<" ";
	else{
	  
	  streamStateSaver.restore();//revert to default printing if no specific precision is required
	  output<<columnSeparator<<" "<<std::setw(labelIterator->size() + minimumColumnWidth)<<Utility::toString(arg)<<" "; 
	  
	}
	++precisionIterator;
	
      }
      else output<<columnSeparator<<" "<<std::setw(labelIterator->size() + minimumColumnWidth)<<Utility::toString(arg)<<" ";
      ++labelIterator;
      print(other...);
      
    }
    else print(); //overlook what is left in 'other' if all the columns have already been filled
    
  }

}
  
#endif
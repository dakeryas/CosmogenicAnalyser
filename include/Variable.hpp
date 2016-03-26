#ifndef COSMOGENIC_ANALYSER_VARIABLE_H
#define COSMOGENIC_ANALYSER_VARIABLE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace CosmogenicAnalyser{

  struct Variable{
    
    double value;
    double error;
    Variable& operator+= (const Variable& other);
    Variable& operator-= (const Variable& other);
    Variable& operator*=(const Variable& other);
    Variable& operator/=(const Variable& other);
    
  };

  Variable operator+(Variable var1, const Variable& var2);
  Variable operator-(Variable var1, const Variable& var2);
  Variable operator*(Variable var1, const Variable& var2);
  Variable operator/(Variable var1, const Variable& var2);
  std::ostream& operator<<(std::ostream& output, const Variable& variable);
  
}

#endif
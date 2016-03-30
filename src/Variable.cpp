#include "Variable.hpp"

namespace CosmogenicAnalyser{

  Variable& Variable::operator+=(const Variable& other){
    
    value += other.value;
    error = std::sqrt(std::pow(error, 2) + std::pow(other.error, 2));
    return *this;

  }

  Variable& Variable::operator-=(const Variable& other){
    
    value -= other.value;
    error = std::sqrt(std::pow(error, 2) + std::pow(other.error, 2));
    return *this;

  }

  Variable& Variable::operator*=(const Variable& other){
    
    error = std::sqrt(std::pow(value * other.error,2)  + std::pow(other.value * error,2) + std::pow(error * other.error, 2)); //sqrt(EX2 varY  + EY2 varX  + varX varY)
    value = value * other.value;
    return *this;

  }

  Variable& Variable::operator/=(const Variable& other){

    if(other.value != 0){
      
      error = value/other.value * std::sqrt(std::pow(error/value,2) + std::pow(other.error/other.value,2));//(EX/EY) sqrt(VarX/EX2 + VarY/EY2)
      value = value / other.value + value * std::pow(other.error,2) / std::pow(other.value, 3);//second order Taylor expansion approximation of E(X/Y)
      
    }
    else throw std::invalid_argument("Division by 0 not allowed!");

    return *this;

  }


  Variable operator+(Variable var1, const Variable& var2){
    
    return var1 += var2;
    
  }

  Variable operator-(Variable var1, const Variable& var2){
    
    return var1 -= var2;
    
  }

  Variable operator*(Variable var1, const Variable& var2){
    
    return var1 *= var2;
    
  }

  Variable operator/(Variable var1, const Variable& var2){
    
    return var1 /= var2;
    
  }

  std::ostream& operator<<(std::ostream& output, const Variable& variable){
    
    output<<variable.value;
    if(variable.value != 0 && variable.error != 0){
      
      auto relativeError = 1e2*variable.error/std::abs(variable.value);
      if(relativeError > 1e3) output<<" (inf)";//1000% error is meaningless
      else{
	
	auto formerPrecision = output.precision();
	output<<" ("<<std::setprecision(1)<<relativeError<<"%)";
	output<<std::setprecision(formerPrecision);
	
      }
      
      
    }
    return output;
    
  }

}
#ifndef INPUT_ITERATOR_H
#define INPUT_ITERATOR_H

#include "cereal/archives/binary.hpp"

namespace cereal{
  
  namespace Archive{

    template <class T>
    class InputIterator : public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, const T*, const T&>{
      
      BinaryInputArchive* inputArchive;
      T value;

    public:
      InputIterator();
      InputIterator(BinaryInputArchive& inputArchive);
      const T& operator*() const;
      const T* operator->() const;
      InputIterator<T>& operator++();
      InputIterator<T> operator++(int);
      bool isEqualTo(const InputIterator<T>& other) const;
      
    };
    
    template<class T>
    InputIterator<T>::InputIterator():inputArchive(nullptr){

    }
    
    template<class T>
    InputIterator<T>::InputIterator(BinaryInputArchive& inputArchive):inputArchive(&inputArchive){
      
      ++*this;

    }
    
    template<class T>
    const T& InputIterator<T>::operator*() const{

      return value;
      
    }
    
    template<class T>
    const T* InputIterator<T>::operator->() const{

      return &value;
      
    }
    
    template<class T>
    InputIterator<T>& InputIterator<T>::operator++(){

      try{
	
	(*inputArchive)(value);
	
      }
      catch(...){

	inputArchive = nullptr;
	
      }

      return *this;
      
    }
    
    template<class T>
    InputIterator<T> InputIterator<T>::operator++(int){
      
      auto copy = *this;
      ++*this;
      return copy;
      
    }

    template<class T>
    bool InputIterator<T>::isEqualTo(const InputIterator<T>& other) const{

      return inputArchive == other.inputArchive;
      
    }

    template<class T>
    bool operator==(const InputIterator<T>& iterator1, const InputIterator<T>& iterator2){

      return iterator1.isEqualTo(iterator2);
      
    }
    
    template<class T>
    bool operator!=(const InputIterator<T>& iterator1, const InputIterator<T>& iterator2){

      return !(iterator1 == iterator2);
      
    }
    
  }

}

#endif
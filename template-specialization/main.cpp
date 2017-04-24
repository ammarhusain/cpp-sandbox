// Copyright (C) 2016 Ammar Husain. All Rights Reserved.

#include <iostream> // std::cout
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

struct MyObj {
  MyObj() { std::cout << "Constructing MyObj." << std::endl; }
};

struct MyObj2 {
  MyObj2() { std::cout << "Constructing MyObj2." << std::endl; }
};

// Template specialize the vector<MyObj2> class to overload the public 'resize'
// function.
// This gives us access to the private members of the vector class, which can
// then be used to move the end vector pointer to its capacity, thereby resizing
// the vector without initializing (constructing individually) each element of
// the array.
// \warning: This specialization is for the libc++ (llvm) standard library.
namespace std {
template <> void vector<MyObj2>::resize(size_t count) {
  std::cout << "Called MyObj2 vector resize\n";
  this->reserve(count);
  this->__end_ = this->__begin_ + count;
}
}

int main(int argc, char **argv) {
  std::vector<MyObj> m_v;
  m_v.resize(10);
  std::cout << "m_v size: " << m_v.size() << std::endl;

  std::vector<MyObj2> m2_v;
  m2_v.resize(10);
  std::cout << "m2_v size: " << m2_v.size() << std::endl;
}

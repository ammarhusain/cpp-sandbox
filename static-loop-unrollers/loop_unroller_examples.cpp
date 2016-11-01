// Copyright (C) 2016 Ammar Husain. All Rights Reserved.

#include <iostream>  // std::cout
#include <string>
#include "loop_unroller.h"

struct MyFunc {
  template<typename I>
  void operator()(I, int val) {
    std::cout << "Functor " << I::value*val << std::endl;
  }
};

int main(int argc, char** argv) {

  // Using functor.
  loop_unroller::ForLoopGreaterThan<10, 1>(MyFunc{}, 5);
  // Using lambda expressions : thanks to c++14!
  loop_unroller::ForLoopLessThanEqualTo<1, 20>([](auto a){
      using INT_CONSTANT=decltype(a);
      std::cout << "Lambda " << INT_CONSTANT::value << std::endl;});

  // Using generic API: prints powers of 2 from [1,6// 4].
  loop_unroller::ForLoop(loop_unroller::LoopParams<1, 64, 2, std::less_equal<int>,
                         std::multiplies<int>>(),
                         [](auto idx, std::string str){
                           using I=decltype(idx);
                           std::cout << str << I::value << std::endl;
                         }, "Power of 2: ");
}

// Copyright (C) 2016 Ammar Husain. All Rights Reserved.

#include <iostream>  // std::cout
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char** argv) {
  std::ostringstream foo_s(std::ios_base::binary);
  // foo_s.exceptions(std::ostream::failbit | std::ostream::badbit | std::ostream::eofbit);
  std::vector<char> bytes(1000, 'a');
  foo_s.write(bytes.data(), bytes.size());

  std::istringstream istr(foo_s.str(), std::ios_base::binary);
  istr.exceptions(std::istream::eofbit);
  try {
    std::vector<char> blah(1000);
    // istr.read(blah.data(), 1000);
    istr.ignore(bytes.size(),  istr.eof());
  } catch(...) {
    std::cout << "exception thrown\n";
  }
}

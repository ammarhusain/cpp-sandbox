// Copyright (C) 2016 Apple Inc. All Rights Reserved.
/// \author: Ammar Husain (mrahusain@gmail.com)

void Foo() {
#if defined(STATIC_TEST_1)
  // This test will fail in the CTest dashboard.
  int a;
  (void) a;
#pragma message "This compiles so test will fail!"
#elif defined(STATIC_TEST_2)
  // Will assert at compile time - test passes.
  constexpr bool val = false;
  static_assert(val, "This line should not compile");
#endif
}

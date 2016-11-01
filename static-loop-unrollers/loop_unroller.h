// Copyright (C) 2016 Ammar Husain. All Rights Reserved.

#pragma once

#include <type_traits>
#include <functional>

using namespace std;

namespace loop_unroller {

// template <bool b, class T = void>
// using enable_if_t = typename std::enable_if<b, T>::type;

// ---
// Loop Params struct

/// \class LoopParams
///
/// \brief Datastructure to hold the parameters of a static looop.
///
/// \tparam CurrIdx Current loop index.
/// \tparam EndIdx End index for the loop.
/// \tparam ChangeVal Change value for each loop increment.
/// \tparam Comparator Comparison functor to check if the loop is within bounds.
/// Compares CurrIdx with EndIdx to determine if the loop is still valid. Eg:
/// less, less than equal to etc. Called as Comparator()(CurrIdx,EndIdx).
/// \tparam Modifier Modification functor that modifies the current loop index
/// (CurrIdx) to a new value given a certain change value (ChangeVal). Eg: add,
/// multiply, divide etc. Called as Modifier()(CurrIdx, ChangeVal).
/// \tparam StartIdx Start index for the loop.
///
template <int CurrIdx, int EndIdx, int ChangeVal = 1,
          class Comparator = less<int>, class Modifier = plus<int>,
          int StartIdx = CurrIdx>
struct LoopParams {
  /// \brief Provides the next incremented LoopParam struct.
  ///
  /// \details This function instantiates a new LoopParams data structure that
  /// holds all the same parameters as the original one, but with an incremented
  /// CurrIdx. The index CurrIdx is incremented based on the modifier (Modifier)
  /// functor and the change (ChangeVal) value of the structure.
  ///
  /// \return  LoopParams datastructure with an incremented CurrIdx, everything
  /// else being the same.
  ///
  static constexpr auto GetNext() noexcept
      -> LoopParams<Modifier()(CurrIdx, ChangeVal), EndIdx, ChangeVal,
                    Comparator, Modifier, StartIdx> {
    return LoopParams<Modifier()(CurrIdx, ChangeVal), EndIdx, ChangeVal,
                      Comparator, Modifier, StartIdx>();
  }

  /// \brief Resets the LoopParams datastructure to its start index.
  ///
  ///
  /// \return LoopParams datastructure with the current index set back to the
  /// start index.
  ///
  static constexpr auto Reset() noexcept
      -> LoopParams<StartIdx, EndIdx, ChangeVal, Comparator, Modifier,
                    StartIdx> {
    return LoopParams<StartIdx, EndIdx, ChangeVal, Comparator, Modifier,
                      StartIdx>();
  }

  /// \brief Checks if the current LoopParams datastructure is within range.
  ///
  /// \details This function checks using the comparison function (Comparator)
  /// whether the current loop index is within the bounds of the loop specified
  /// with an end index EndIdx.
  ///
  /// \return True if the current LoopParams datastructure is within bounds,
  /// false otherwise.
  ///
  static constexpr bool IsValid() noexcept {
    return Comparator()(CurrIdx, EndIdx);
  }

  /// \brief Gets the current loop index.
  ///
  /// \return Current loop index.
  ///
  static constexpr int GetCurrIdx() noexcept { return CurrIdx; }

  /// \brief Gets the end loop index.
  ///
  /// \return End loop index.
  ///
  static constexpr int GetEndIdx() noexcept { return EndIdx; }

  /// \brief Gets the loop change value
  ///
  /// \return Loop change value.
  ///
  static constexpr int GetChangeVal() noexcept { return ChangeVal; }

  /// \brief Gets the comparison functor.
  ///
  /// \return Comparison Functor.
  ///
  static constexpr Comparator GetComparator() noexcept { return Comparator(); }

  /// \brief Gets the modifying functor.
  ///
  /// \return Modifying Functor.
  ///
  static constexpr Modifier GetModifier() noexcept { return Modifier(); }

  /// \brief Gets the loop start index.
  ///
  /// \return Loop start index.
  ///
  static constexpr int GetStartIdx() noexcept { return StartIdx; }
};

// ---
// Generic API
/// \defgroup GenericForLoop ForLoop (Generic API)
///
/// \brief Compile-time loop unroller for simple for-loops.
///
/// \details
/// This function recursively unrolls a loop within a start and end index. The
/// user must specify the loop properties in a LoopParams datastructure. The
/// configurable properties are the: (i) start & (ii) end indices, (iii) change
/// values, (iv) a comparison functor and (v) a modifying functor. The
/// comparison functor returns a bool and is used to check whether the current
/// loop index is within the bounds of the start & end indices (Eg: less than,
/// greater than or equal to etc.). The modifier functor increments the current
/// loop index to a new loop index, given its change value, and returns the new
/// loop index (Eg: plus, minus, multiplies, divides etc.).
/// Recursion for the ForLoop function is stopped
/// using the enable_if concept of SFINAE (Substitution Failure Is Not An Error)
/// that checks where the LoopParams structure being currently operated on is
/// within range of the start & end indices (using the comparator).
/// \n
/// The loop unroller expects a user defined functor/ lambda
/// that it can call at each iteration of the loop. An
/// std::integral_constant containing the value of the current loop index is
/// passed as the first argument to the functor being invoked. Users are free to
/// define multiple additional arguments in the functor that are passed through
/// variadic templates with perfect forwarding.
/// Example code:
/// \code{.cpp}
/// loop_unroller::ForLoop(loop_unroller::LoopParams<1, 64, 2, std::less_equal<int>,
/// std::multiplies<int>>(),
/// [](auto idx, float a){
/// using I=decltype(idx);
///     DiagonalMatrix<float, I::value> d_mat;
///     d_mat.diagonal() = Vector<I::value>::Ones * a;
/// }, 3.14);
/// \endcode
/// \n
/// The function object above uses the
/// integral_constant to instantitate a square matrix of size equal to the
/// current loop index. It also takes in a floating point number a (passed in
/// with variadic number of arguments) that is used to initialize the diagonal
/// elements of the square matrix to.
///
/// \return Void.
///
/// \{
template <class LP, class Functor, typename... Args>
enable_if_t<!LP::IsValid()> ForLoop(LP, Functor func, Args &&... args) {}

template <class LP, class Functor, typename... Args>
enable_if_t<LP::IsValid()> ForLoop(LP, Functor func, Args &&... args) {
  std::integral_constant<int, LP::GetCurrIdx()> i;
  func(i, std::forward<Args>(args)...);
  return ForLoop(LP::GetNext(), func, std::forward<Args>(args)...);
}
/// \}

// ---
// Specialized API
/// \defgroup SpecializedForLoop ForLoop (Specialized API)
///
/// \brief Simpler interface for the compile-time loop unroller.
///
/// \details
/// Following are specialized compile time ForLoop functions that default the
/// comparison operator to either (i) less than, (ii) less than equal to, (iii)
/// greater than, (iv) greater than equal to or (v) equal to. The modifying
/// function is set to the plus operator. Change value defaults to -1 when the
/// comparator is greater than or greater than equal to, and +1 otherwise.
/// \n
/// Sample usage:
/// \code{.cpp}
/// ForLoopGreaterThan<10, 1>(MyFunctor{}, args...);
/// \endcode
/// \n
/// The user may specify the change value as an additional template parameter.
/// For example the code below increments the loop by 2 at each step between [0,
/// 10]:
/// \code{.cpp}
/// ForLoopLessEqualTo<0, 10, 2>(MyFunctor{}, args...);
/// \endcode
///
///
/// \param[in] func User defined functor to invoke at each loop iteration.
/// \param[in] args Variable number of arguments forwarded to func.
/// \tparam    StartIdx Loop start index.
/// \tparam    EndIdx Loop end index.
/// \tparam    ChangeVal Value to change at each loop increment. Defaults to 1
/// (or -1).
///
/// \{

/// \brief Uses the less than comparison operator.
template <int StartIdx, int EndIdx, int ChangeVal = 1, class Functor,
          typename... Args>
void ForLoopLessThan(Functor func, Args &&... args) {
  return ForLoop(
      LoopParams<StartIdx, EndIdx, ChangeVal, less<int>, plus<int>, StartIdx>(),
      func, std::forward<Args>(args)...);
}

/// \brief Uses the less than or equal to comparison operator.
template <int StartIdx, int EndIdx, int ChangeVal = 1, class Functor,
          typename... Args>
void ForLoopLessThanEqualTo(Functor func, Args &&... args) {
  return ForLoop(LoopParams<StartIdx, EndIdx, ChangeVal, less_equal<int>,
                            plus<int>, StartIdx>(),
                 func, std::forward<Args>(args)...);
}

/// \brief Uses the greater than comparison operator.
template <int StartIdx, int EndIdx, int ChangeVal = -1, class Functor,
          typename... Args>
void ForLoopGreaterThan(Functor func, Args &&... args) {
  return ForLoop(LoopParams<StartIdx, EndIdx, ChangeVal, greater<int>,
                            plus<int>, StartIdx>(),
                 func, std::forward<Args>(args)...);
}

/// \brief Uses the greater than or equal to comparison operator.
template <int StartIdx, int EndIdx, int ChangeVal = -1, class Functor,
          typename... Args>
void ForLoopGreaterThanEqualTo(Functor func, Args &&... args) {
  return ForLoop(LoopParams<StartIdx, EndIdx, ChangeVal, greater_equal<int>,
                            plus<int>, StartIdx>(),
                 func, std::forward<Args>(args)...);
}

/// \brief Uses the equal to comparison operator.
template <int StartIdx, int EndIdx, int ChangeVal = 1, class Functor,
          typename... Args>
void ForLoopEqualTo(Functor func, Args &&... args) {
  return ForLoop(LoopParams<StartIdx, EndIdx, ChangeVal, equal_to<int>,
                            plus<int>, StartIdx>(),
                 func, std::forward<Args>(args)...);
}
/// \}

// ---
// Nested Loop API
/// \defgroup NestedForLoop_2 2 NestedForLoop
///
/// \brief Metafunctions to unroll 2 nested for loops.
///
/// \details
/// NextedForLoop_2 provides a mechanism to statically unroll 2 nested loops.
/// The user must specify the loop properties, using the LoopParams
/// datastructure, for the outer and inner loop. Additionally the metafunction
/// also expects a user defined functor to invoke at each loop iteration as well
/// as variadic arguments that the passed in functor expects to receive. The
/// user defined functor must accept 2 std::integral_constant types as its first
/// & second argument. The first argument holds the static type value (inside
/// integral_constant) of the inner loop index while the second argument holds
/// the static type value of the outer loop index. Variadic arguments are
/// forwarded to the functor after the first 2 arguments.
/// \n
/// Example:
/// \code{.cpp}
/// loop_unroller::NestedForLoop_2(loop_unroller::LoopParams<0, 10>(), loop_unroller::LoopParams<10, 0,
/// -1>(), MyFunctor{});
/// \endcode
/// \n
/// Sample functor that gets invoked:
/// \code{.cpp}
/// struct MyFunctor {
///   template<typename I, typename J>
///   operator()(I, J, MatrixXd& mat) {
///     // I::value is the value of the current inner loop index
///     // J::value is the value of the current outer loop index.
///     mat.resize(I::value, J::value);
///     // some operations on mat here.
///   }
/// }
/// \endcode
///
/// \{
template <class LP_out, class LP_in, class Functor, typename... Args>
enable_if_t<!LP_out::IsValid()> NestedForLoop_2(LP_out, LP_in, Functor func,
                                                Args &&... args) {}

template <class LP_out, class LP_in, class Functor, typename... Args>
enable_if_t<LP_out::IsValid() && LP_in::IsValid()>
NestedForLoop_2(LP_out p, LP_in q, Functor func, Args &&... args) {
  ForLoop(q, func, std::integral_constant<int, LP_out::GetCurrIdx()>(),
          std::forward<Args>(args)...);
  NestedForLoop_2(LP_out::GetNext(), LP_in::Reset(), func, std::forward<Args>(args)...);
}
/// \}

}  // namespace loop_unroller

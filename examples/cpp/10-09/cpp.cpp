#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <cmath>
#include <array>
#include <deque>
#include <forward_list>
#include <map>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <bitset>

#include <type_traits>
#include <typeinfo>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

template <class T>
std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

constexpr double square(double x) {return (x * x);}

int main()
{
  constexpr double x1 = square(1.6);
  constexpr double x2 = 1.8 * square(1.2);
  constexpr double x3 = x1 * x2;

  std::cout << x3 << std::endl;

  for (auto x: {5, 3, 1, 6, 7}) std::cout << x << std::endl;

  int y1 = 3;
  double y2 = 4.4;

  int &yref = y1;
  std::cout << yref << std::endl;

  yref = y2;
  std::cout << yref << std::endl;
  std::cout << y1 << std::endl;

  return(0);
}

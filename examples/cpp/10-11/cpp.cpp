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
#include <initializer_list>

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

template <class T>
class Some_Vector
{
public:
  Some_Vector(const std::initializer_list<T> &list) : v(list)
  {
  }

private:
  std::vector<T> v;
};

int main()
{
  std::vector<std::unique_ptr<int>> vs;
  std::unique_ptr<int> uniq = std::make_unique<int>(3);
  std::unique_ptr<int> other = std::move(uniq);

  vs.push_back(std::make_unique<int>(10));
  vs.push_back(std::make_unique<int>(11));

  for (const auto &i: vs)
  {
    std::cout << *i << std::endl;
  }

  return(0);
}

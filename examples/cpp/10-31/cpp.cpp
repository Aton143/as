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
#include <iterator>

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
class Vector
{
public:
  Vector(size_t count, T start_value) noexcept
    : __count(count), __used(0)
  {
    __data = reinterpret_cast<T *>(std::malloc(count * sizeof(T)));
    for (size_t i = 0; i < count; ++i)
    {
      __data[i] = start_value;
    }
  }

  Vector(void) noexcept
    : __count(0), __data(nullptr), __used(0)
  {
  }

  ~Vector() noexcept
  {
    delete[] __data;
  }

  Vector &push_back(T &new_value)
  {
    if (__used >= __count)
    {
      __data = reinterpret_cast<T *>(std::realloc(__data, __count * 2));
      __count *= 2;
    }

    __data[__used++] = new_value;
    return(*this);
  }

  Vector &push_back(T new_value)
  {
    if (__used >= __count)
    {
      __data = reinterpret_cast<T *>(std::realloc(__data, __count * 2));
      __count *= 2;
    }

    __data[__used++] = new_value;
    return(*this);
  }

  T *begin(void) noexcept
  {
    return(__data);
  }

  T *end(void) noexcept
  {
    return(__data + __used);
  }

private:
  T      *__data;
  size_t  __used, __count;
};

int main()
{
  Vector<double> vi(10, 10);
  Vector<double> v(10, 10);

  v.push_back(0);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  v.push_back(7);
  v.push_back(8);
  v.push_back(9);
  v.push_back(10);

  return(0);
}

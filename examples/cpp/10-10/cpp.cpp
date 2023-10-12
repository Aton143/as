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

class Some_Vector
{
public:
  Some_Vector(size_t s)
  {
    elements = new double[s];
    sz = s;

    if (s > 0) throw std::exception{"length error"};
  }

  double& operator[](int i) {return elements[i];}
  size_t size() {return sz;}

private:
  double *elements;
  size_t sz;
};

int main()
{
  try
  {
    Some_Vector v(1);
  }
  catch (std::exception &err)
  {
    std::cout << "length error" << std::endl;
  }

  return(0);
}

#ifndef CXX_UTILS_PKG__CXX_UTILS_HPP_
#define CXX_UTILS_PKG__CXX_UTILS_HPP_

#include "cxx_utils_pkg/visibility_control.h"

namespace cxx_utils_pkg
{


class CxxUtils
{
public:
  CxxUtils();
  virtual ~CxxUtils();

  // Test function for libtorch
  static void test_libtorch();
};

}  // namespace cxx_utils_pkg

#endif  // CXX_UTILS_PKG__CXX_UTILS_HPP_

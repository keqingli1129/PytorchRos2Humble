#include <iostream>
#include <torch/torch.h>
#include "cxx_utils_pkg/cxx_utils.hpp"

namespace cxx_utils_pkg
{

CxxUtils::CxxUtils()
{
}

CxxUtils::~CxxUtils()
{
}

void cxx_utils_pkg::CxxUtils::test_libtorch() {
	// Simple tensor operation to test libtorch
	torch::Tensor tensor = torch::rand({2, 3});
	std::cout << "Libtorch tensor test: " << tensor << std::endl;
}

}  // namespace cxx_utils_pkg

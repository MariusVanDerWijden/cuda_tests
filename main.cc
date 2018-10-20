#include <iostream>
#include <iomanip>

#include "dagtest.h"

int main(int argc, char **argv)
{
	std::cout << "Test" << std::endl;

	print_cuda_devices();
	perf_test();
}

#include <iostream>
#include <thread>

#include "Array/Array.h"



int main()
{
  try{
    array::test();
  }
	catch (const std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Undefined exception" << std::endl;
	}
  return EXIT_SUCCESS;
}
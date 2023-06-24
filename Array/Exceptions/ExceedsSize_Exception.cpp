#include "ExceedsSize_Exception.h"


ExceedsSize_Exception::ExceedsSize_Exception(size_t size,
                                            size_t id): std::exception(),
  errorMessage_("Error: id (" +
  std::to_string(id) + ") out of range (" + std::to_string(size) + ")")                                
{
}



const char* ExceedsSize_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}
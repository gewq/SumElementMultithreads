#include "StartIdExceedsEndId_Exception.h"


StartIdExceedsEndId_Exception::StartIdExceedsEndId_Exception(size_t startID,
  size_t endID): std::exception(),
  errorMessage_("Error: startID (" +
  std::to_string(startID) + ") > endID (" + std::to_string(endID) + ")")                                
{
}



const char* StartIdExceedsEndId_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}
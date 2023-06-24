/**
\file StartIdExceedsEndId_Exception.h
\brief Класс StartIdExceedsEndId_Exception - класс-обработчик исключения "Индекс Начала больше индекса Конца"
*/

#pragma once

#include <string>
#include <exception>

class StartIdExceedsEndId_Exception : public std::exception {
  public:
    StartIdExceedsEndId_Exception() = delete;

    /**
    \param[in] startID Индекс начала
    \param[in] endID Индекс конца
    */
    explicit StartIdExceedsEndId_Exception(size_t startID, size_t endID);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};
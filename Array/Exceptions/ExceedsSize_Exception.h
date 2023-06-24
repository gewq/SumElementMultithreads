/**
\file ExceedsSize_Exception.h
\brief Класс ExceedsSize_Exception - класс-обработчик исключения "Индекс за пределами массива"
*/

#pragma once

#include <string>
#include <exception>

class ExceedsSize_Exception : public std::exception {
  public:
    ExceedsSize_Exception() = delete;

    /**
    \param[in] size Размер массива
    \param[in] id Значение индекса превышающего размер
    */
    explicit ExceedsSize_Exception(size_t size, size_t id);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};
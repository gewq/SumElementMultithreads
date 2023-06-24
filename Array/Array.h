/**
\file Array.h
\brief Модуль - выполняет суммирование элементов массива 
*/

#pragma once

#include <iostream>


namespace array{
  /**
  Суммирует элементы массива
  \param[in] array Массив элементов
  \param[in] size Количество элементов в массиве
  \return Сумму элементов массива
  */
  template <typename T>
  T calculateSumma(const T array[], size_t size);

  /**
  Протестировать методы модуля
  */
  void test();
}
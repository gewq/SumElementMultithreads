#include "Array.h"

#include <thread>
#include <future>
#include <vector>
#include <assert.h>

#include "Exceptions/ExceedsSize_Exception.h"
#include "Exceptions/StartIdExceedsEndId_Exception.h"


namespace{
  //Максимальное количество потоков, обеспечиваемое аппаратной частью
  const uint8_t MAX_THREADS = std::thread::hardware_concurrency();

  //Минимальное количество элементов, обрабатываемых одним потоком
  //Чтобы не создавать например 10 потоков для 10 элементов
  const size_t MIN_ELEMENTS_PER_THREAD = 1'000;
}


/**
Вычислить сумму элементов массива в заданном интервале индексов
\param[in] array Массив элементов
\param[in] size Количество элементов в массиве
\param[in] startID Индекс начиная с которого суммировать
\param[in] endID Индекс конечного элемента который не входит в суммирование
\return Сумму элементов диапазона
*/
template <typename T>
static T calculateSumOfRange(const T array[], size_t size,
                            size_t startID, size_t endID);


/**
Суммирует элементы массива одно- или многопоточно-
\param[in] array Массив элементов
\param[in] size Количество элементов в массиве
\return Сумму элементов массива
*/
template <typename T>
T array::calculateSumma(const T array[], size_t size)
{
  T result = 0;

  //Однопоточная версия
  if ( (std::thread::hardware_concurrency() < 2) ||
        (size <= MIN_ELEMENTS_PER_THREAD) ){
    result = calculateSumOfRange(array, size, 0, size);
  }

  //Многопоточная версия
  //Исходный массив разбивается по M элементов - кол-во потоков
  //Каждый поток суммирует свою часть
  //Результаты полученные потоками суммируются
  else{
    //Количество потоков суммирующих отдельные части
    size_t nThreads = std::min<size_t>(
                                size / MIN_ELEMENTS_PER_THREAD, MAX_THREADS);

    //Количество элементов для суммирования каждым потоком
    size_t batchSize = size/nThreads;
    if (size % MIN_ELEMENTS_PER_THREAD != 0){
      nThreads += 1;
    }

    //Результаты суммирования потоками
    std::vector< std::future<T> > sumByThreads;
    sumByThreads.reserve(nThreads);

    //Поставить задачи каждому потоку
    for (size_t i=0; i<size; i += batchSize){   
      //Последнему потоку суммировать до конца массива
      if ((i + batchSize) > size){
        sumByThreads.push_back(
          std::async(std::launch::async,
                    calculateSumOfRange<T>,
                    array, size, i, size
          )
        );
      }

      //Остальным по batchSize элементов
      else{
        sumByThreads.push_back(
          std::async(std::launch::async,
                    calculateSumOfRange<T>,
                    array, size, i, i+batchSize
          )
        );
      }
    }

    //Просуммировать результаты каждого потока по мере готовности 
    for (auto& sum: sumByThreads){
      if (sum.valid()){
        result += sum.get();
      }
    }
  }
  return result;
}


//-----------------------------------------------------------------------------
void array::test()
{
  int testArray_1[] = {1,2,3,4,5,6,-7,8,9,10,11,12,-13,14,15,16,-17,18,
    19,20,21,22,23,24,25};
  assert(calculateSumOfRange<int>(testArray_1, std::size(testArray_1),
                                  0, std::size(testArray_1)) == 251);
  assert(calculateSumOfRange<int>(testArray_1, std::size(testArray_1),
                                  1, 1) == 2);
  assert(calculateSumma<int>(testArray_1, std::size(testArray_1)) == 251);


  const float EPS = 0.001;  //Точность с которой проверяется равенство float
  float testArray_2[] = {1.0,2.0,3.0,4.0,5.0,6.0,-7.0,8.0,9.0,10.0,11.0,
    12.0,-13.0,14.0,15.0,16.0,-17.0,18.0,19.0,20.0,21.0,22.0,23.0,24.0,25.0};
  assert(calculateSumOfRange<float>(testArray_2, std::size(testArray_2),
                                    0, std::size(testArray_1)) == 251.0);
  assert( (calculateSumOfRange<float>(testArray_2,std::size(testArray_2),
                                    1, 1) - 2.0) < EPS );
  assert( (calculateSumma<float>(testArray_2,
                                    std::size(testArray_2)) - 251.0) < EPS);

  //Выброс исключений 
  //Out of range
  // calculateSumOfRange<int>(testArray_1, std::size(testArray_1),
  //                           1, std::size(testArray_1)+1);
  //Start_ID > End_ID
  // calculateSumOfRange<float>(testArray_2, std::size(testArray_2), 10, 6);
}



//=============================================================================
template <typename T>
static T calculateSumOfRange(const T array[], size_t size, size_t startID, size_t endID)
{
  if (startID > endID){
    throw StartIdExceedsEndId_Exception(startID, endID);
  }
  if (startID > size-1) {
    throw ExceedsSize_Exception(size, startID);
  }
  if (endID > size) {
    throw ExceedsSize_Exception(size, endID);
  }
  if (startID == endID){
    return array[startID];
  }

  T summa = 0;
  for(size_t i=startID; i<endID; ++i){
    summa += array[i];
  }
  return summa;
}
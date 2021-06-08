#pragma once
#include <freertos/FreeRTOS.h>
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "./IConcurrencyPlatform.hpp"

namespace SimpleFSM {
  struct FreeRTOSConcurrencyPlatform : public IConcurrencyPlatform {
    struct Queue : public IConcurrencyPlatform::Queue {
      Queue(unsigned int elements, unsigned int elementSize);
      bool push(void *element, uint32_t timeout);
      bool pop(void *element, uint32_t timeout);
     private:
      QueueHandle_t _queue;
    };
    virtual IConcurrencyPlatform::Queue *makeQueue(unsigned int elements, unsigned int elementSize);

    struct Mutex : public IConcurrencyPlatform::Mutex {
      Mutex();
      bool take(uint32_t timeout);
      bool give();
     private:
      SemaphoreHandle_t _mutex;
    };
    virtual IConcurrencyPlatform::Mutex *makeMutex();
  };
  
  /* Implementation */

  /* Queue */
  inline FreeRTOSConcurrencyPlatform::Queue::Queue(unsigned int elements, unsigned int elementSize) {
    _queue = xQueueCreate(elements, elementSize);
  }

  inline bool FreeRTOSConcurrencyPlatform::Queue::push(void *element, uint32_t timeoutMs) {
    return xQueueSendToBack(_queue, element, timeoutMs / portTICK_PERIOD_MS) == pdTRUE;
  }

  inline bool FreeRTOSConcurrencyPlatform::Queue::pop(void *element, uint32_t timeoutMs) {
    return xQueueReceive(_queue, element, timeoutMs / portTICK_PERIOD_MS) == pdTRUE;
  }

  inline IConcurrencyPlatform::Queue *
  FreeRTOSConcurrencyPlatform::makeQueue(unsigned int elements, unsigned int elementSize) {
    return new FreeRTOSConcurrencyPlatform::Queue(elements, elementSize);
  }

  /* Mutex */
  inline FreeRTOSConcurrencyPlatform::Mutex::Mutex() {
    _mutex = xSemaphoreCreateRecursiveMutex();
  }

  inline bool FreeRTOSConcurrencyPlatform::Mutex::take(uint32_t timeoutMs) {
    return xSemaphoreTakeRecursive(_mutex, timeoutMs / portTICK_PERIOD_MS) == pdTRUE;
  }

  inline bool FreeRTOSConcurrencyPlatform::Mutex::give() {
    return xSemaphoreGiveRecursive(_mutex);
  }

  inline IConcurrencyPlatform::Mutex *
  FreeRTOSConcurrencyPlatform::makeMutex() {
    return new FreeRTOSConcurrencyPlatform::Mutex();
  }

};

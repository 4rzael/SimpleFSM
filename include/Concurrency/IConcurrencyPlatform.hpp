#pragma once
#include <ctype.h>

namespace SimpleFSM {
  struct IConcurrencyPlatform {
    struct Queue {
      virtual bool push(void *element, uint32_t timeoutMs);
      virtual bool pop(void *element, uint32_t timeoutMs);
    };
    virtual Queue *makeQueue(uint32_t elements, uint32_t elementSize) = 0;

    struct Mutex {
      virtual bool take(uint32_t timeoutMs);
      virtual bool give();
    };
    virtual Mutex *makeMutex() = 0;
  };
};

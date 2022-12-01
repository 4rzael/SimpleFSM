#pragma once
#include "./IConcurrencyPlatform.hpp"

namespace SimpleFSM {
  /**
   * @brief A RAII-based tool to safely lock/unlock mutexes
   */
  class LockContext {
  public:
    LockContext(IConcurrencyPlatform::Mutex *mutex): _mutex(mutex) {
      while (!_mutex->take(1)) {}  // Requires OS frequency >= 1kHz
    }
    // Can be called to unlock before destruction
    void unlock() { 
      if (_locking) {
        _locking = false;
        _mutex->give();
      }
    }
    ~LockContext() {
      unlock();
    }
   private:
    bool _locking = true;
    IConcurrencyPlatform::Mutex *_mutex;
  };
};

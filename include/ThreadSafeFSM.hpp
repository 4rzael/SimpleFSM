#pragma once
#include "./SimpleFSM.hpp"
#include "./Concurrency/LockContext.hpp"

namespace SimpleFSM {
  template <class Base,
            class StateEnum, class EventEnum, class EventPayload_t,
            class ConcurrencyPlatform, unsigned int EVENT_QUEUE_SIZE = 0>
  class ThreadSafeFSM : public Base {
   private:
    struct QueuedEvent {
      EventEnum event;
      EventPayload_t payload;
    };
   public:
    using EventPayload = EventPayload_t;
    ThreadSafeFSM() : Base() {
      _eventQueue = nullptr;
      _mutex = _concurrencyPlatform.makeMutex();
      if constexpr (EVENT_QUEUE_SIZE > 0) {
        _eventQueue = _concurrencyPlatform.makeQueue(EVENT_QUEUE_SIZE, sizeof(QueuedEvent));
      }
    }

    FSMError start(StateEnum initialState) {
      LockContext lock(_mutex);
      return Base::start(initialState);
    }

    /**
     * @brief Transitions to a next state.
      * This will be replaced by a templated method to be able to build .dot files (and will thus be deprecated soon)
      * @param newState The state to transition to
      */
    FSMError transit(StateEnum newState) {
      LockContext lock(_mutex);
      return Base::transit(newState);
    }

    /**
     * @brief Emits an event to the FSM. States can react to those events.
      * Note: This function is synchronous if EVENT_QUEUE_SIZE is set to 0
      * Otherwise, it is asynchronous, waiting at most 1ms, with a possibility of a full queue error
      * 
      * @param event The event to dispatch
      * @param payload The event payload
      * @return FSMError 
      */
    FSMError emit(EventEnum event, EventPayload const &payload) {
      if constexpr (EVENT_QUEUE_SIZE != 0) {
        // Only queue the event, to make it asynchronous
        QueuedEvent ev = {.event = event, .payload = payload};
        if (_eventQueue->push(&ev, 1)) {
          return FSMError::OK;
        } else {
          return FSMError::ASYNC_OPERATION_ERROR;
        }
      } else {
        LockContext lock(_mutex);
        return Base::emit(event, payload);
      }
    }

    FSMError emit(EventEnum event) {
      constexpr bool payloadIsEmpty = ::std::is_same<EventPayload, EmptyPayload>::value;
      static_assert(payloadIsEmpty, "Cannot call emit() without a payload if the FSM events have a payload");
      return emit(event, EventPayload());
    }

    /**
     * @brief Updates the FSM. Calling the loop() from the current state
      */
    FSMError update() {
      if constexpr (EVENT_QUEUE_SIZE != 0) {
        QueuedEvent ev;
        for (uint i = 0; i < EVENT_QUEUE_SIZE; ++i) {
          if (!_eventQueue->pop(&ev, 0))
            break;
          Base::emit(ev.event, ev.payload);
        }
      }

      {
        LockContext lock(_mutex);
        return Base::update();
      }
    }

    StateEnum getCurrentState() const {
      LockContext lock(_mutex);
      return Base::getCurrentState();
    }
   private:
    ConcurrencyPlatform                    _concurrencyPlatform;
    typename IConcurrencyPlatform::Queue  *_eventQueue;
    typename IConcurrencyPlatform::Mutex  *_mutex;
  };
};  // namespace SimpleFSM

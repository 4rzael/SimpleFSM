#pragma once
#include <SimpleFSM.hpp>

namespace SimpleFSM {
  template <class Base,
            class StateEnum, class EventEnum, class EventPayload_t=EmptyPayload>
  class HookableFSM: public Base {
  public:
    using TransitionHook = std::function<void (StateEnum from, StateEnum to)>;
    using EventHook = std::function<void (EventEnum event, EventPayload_t const &payload)>;

    void onTransition(TransitionHook const &hook) { _transitionHooks.push_back(hook); }
    void onEvent(EventHook const &hook) { _eventHooks.push_back(hook); }

    FSMError emit(EventEnum event, EventPayload_t const &payload) {
      auto result = Base::emit(event, payload);
      if (result == FSMError::OK) {
        for (auto const &hook : _eventHooks) {
          hook(event, payload);
        }
      }
      return result;
    }

    FSMError emit(EventEnum event) {
      return emit(event, EventPayload_t());
    }

    FSMError transit(StateEnum newState) {
      auto oldState = Base::getCurrentState();
      auto result = Base::transit(newState);
      if (result == FSMError::OK) {
        for (auto const &hook : _transitionHooks) {
          hook(oldState, newState);
        }
      }
      return result;
    }

  private:
    std::vector<TransitionHook>  _transitionHooks;
    std::vector<EventHook>       _eventHooks;
  };
}
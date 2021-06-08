#pragma once
#include <functional>
#include <type_traits>
#include <vector>

namespace SimpleFSM {
  /**
   * @brief Errors returned by the library
   * TODO: Make the library call an optional function when an error happens, to avoid having to check errors on every single call
   */
  enum class FSMError {
    OK,
    FSM_ALREADY_STARTED,
    FSM_NOT_STARTED,
    BAD_STATE,
    STATE_ALREADY_SET,
    MISSING_STATE,
    INVALID_PERMISSION,
    ASYNC_OPERATION_ERROR,
  };

  struct EmptyPayload {};

  /**
   * @brief The Finite State Machine
   * 
   * @tparam StateEnum An enum class containing all states supported by the FSM.
   *  The enum MUST be unsigned only
   *  The enum elements MUST start with 0 and be consecutive
   *  The last element MUST be called _SIMPLE_FSM_INVALID_
   * @tparam EventEnum An enum class containing all events supported by the FSM
   */
  template <class StateEnum, class EventEnum, class EventPayload_t=EmptyPayload>
  class FSM {
    public:
      using EventPayload = EventPayload_t;

    private:
    // A simple helper
    template<class T>
    constexpr static size_t to_size_t(T v) { return static_cast<size_t>(v); };

    public:
      /**
       * @brief The base class for the FSM's state.
       * It is virtual pure, so needs to be inherited
       */
      class State {
      public:
        State(StateEnum state): _state(state) {}
        StateEnum getValue() const { return _state; }

        virtual void entry() = 0;
        virtual void loop() = 0;
        virtual void exit() = 0;
        virtual void react(EventEnum event, EventPayload const &payload) = 0;
        virtual char const *getName() { return "?"; }
      private:
        StateEnum _state;
      };

      /**
       * @brief Adds a state to the FSM.
       * All states in the enum MUST be added once and only once to the FSM, 
       * @param state A pointer to the State to add
       */
      FSMError addState(State *state) {
        if (!state) return FSMError::BAD_STATE;
        auto s = state->getValue();
        if (_started) return FSMError::FSM_ALREADY_STARTED;
        if (s >= StateEnum::_SIMPLE_FSM_INVALID_) return FSMError::BAD_STATE; //TODO: Make sure enums are unsigned
        if (getStatePointer(s) != nullptr) return FSMError::STATE_ALREADY_SET;
        _states[to_size_t(s)] = state;
        return FSMError::OK;
      }

      /**
       * @brief Freezes the states & transitions, and starts calling the states methods
       * 
       * @param initialState The state to start the FSM in
       */
      FSMError start(StateEnum initialState) {
        for (size_t i = 0; i < to_size_t(StateEnum::_SIMPLE_FSM_INVALID_); ++i) {
          if (_states[i] == nullptr) return FSMError::MISSING_STATE;
        }

        _initialState = initialState;
        _currentState = _initialState;
        _started = true;
        getStatePointer(_currentState)->entry();
        return FSMError::OK;
      }

      /**
       * @brief Resets the FSM to its initial state, even if no transition to that state are available
       */
      FSMError reset() {
        return transit(_initialState);
      }

      /**
       * @brief Transitions to a next state.
       * This will be replaced by a templated method to be able to build .dot files (and will thus be deprecated soon)
       * @param newState The state to transition to
       */
      FSMError transit(StateEnum newState) {
        if (!_started) return FSMError::FSM_NOT_STARTED;
        getStatePointer(_currentState)->exit();
        _currentState = newState;
        getStatePointer(_currentState)->entry();
        return FSMError::OK;
      }

      /**
       * @brief Emits an event to the FSM. States can react to those events.
       * Note: This function is synchronous.
       * 
       * @param event The event to dispatch
       * @param payload The event payload
       * @return FSMError 
       */
      FSMError emit(EventEnum event, EventPayload const &payload) {
        if (!_started) return FSMError::FSM_NOT_STARTED;

        getStatePointer(_currentState)->react(event, payload);
        return FSMError::OK;
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
        if (!_started) return FSMError::FSM_NOT_STARTED;
        getStatePointer(_currentState)->loop();
        return FSMError::OK;
      }

      StateEnum getCurrentState() const { return _currentState; }
      State    *getStatePointer(StateEnum s) { return _states[to_size_t(s)]; }

    private:
      bool      _started = false;
      StateEnum _initialState = StateEnum::_SIMPLE_FSM_INVALID_;
      StateEnum _currentState = StateEnum::_SIMPLE_FSM_INVALID_;
      State    *_states[to_size_t(StateEnum::_SIMPLE_FSM_INVALID_)] = {nullptr};
  };
};

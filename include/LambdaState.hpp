#pragma once
#include <SimpleFSM.hpp>

namespace SimpleFSM {
  /**
   * @brief An implementation of the class State, that can be built by giving it lambdas.
   * This allows to create small FSMs with few boilerplate
   */
  template <class StateEnum, class EventEnum, class EventPayload=EmptyPayload>
  class LambdaState : public FSM<StateEnum, EventEnum, EventPayload>::State {
  private:
    using BaseState = typename FSM<StateEnum, EventEnum, EventPayload>::State;

  public:
    using EntryFunction = std::function<void ()>;
    using ReactFunction = std::function<void (EventEnum event, EventPayload const &payload)>;
    using ExitFunction  = std::function<void ()>;
    using LoopFunction  = std::function<void ()>;

    struct Config {
      EntryFunction entry;
      ReactFunction react;
      ExitFunction  exit;
      LoopFunction  loop;
    };

    LambdaState(StateEnum state, Config const &cfg)
    : BaseState(state), _entry(cfg.entry), _react(cfg.react), _exit(cfg.exit), _loop(cfg.loop) {}

    virtual void entry()                                             { if (_entry) _entry(); }
    virtual void loop()                                              { if (_loop)  _loop(); }
    virtual void exit()                                              { if (_exit)  _exit(); }
    virtual void react(EventEnum event, EventPayload const &payload) { if (_react) _react(event, payload); }

  private:
    EntryFunction _entry;
    ReactFunction _react;
    ExitFunction  _exit;
    LoopFunction  _loop;
  };
};
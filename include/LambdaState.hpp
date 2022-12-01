#pragma once
#include <SimpleFSM.hpp>

namespace SimpleFSM {
  /**
   * @brief An implementation of the class State, that can be built by giving it lambdas.
   * This allows to create small FSMs with few boilerplate
   */
  template <class StateEnum, class EventEnum, class EventPayload=EmptyPayload,
            typename Base = typename FSM<StateEnum, EventEnum, EventPayload>::State>
  class LambdaState : public Base {
  private:
    using BaseState = Base;

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

    LambdaState(StateEnum state, Config const &cfg, char const *name=nullptr)
    : BaseState(state), _entry(cfg.entry), _react(cfg.react), _exit(cfg.exit), _loop(cfg.loop), _name(name) {}

    virtual void entry()                                             { if (_entry) _entry(); }
    virtual void loop()                                              { if (_loop)  _loop(); }
    virtual void exit()                                              { if (_exit)  _exit(); }
    virtual void react(EventEnum event, EventPayload const &payload) { if (_react) _react(event, payload); }
    virtual char const *getName()                                    { return _name ? _name : BaseState::getName(); }

  private:
    EntryFunction _entry;
    ReactFunction _react;
    ExitFunction  _exit;
    LoopFunction  _loop;
    char const    *_name;
  };
};
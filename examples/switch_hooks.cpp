#include <iostream>
#include "SimpleFSM.hpp"
#include "HookableFSM.hpp"
#include "LambdaState.hpp"

enum class States {
  ON,
  OFF,
  _SIMPLE_FSM_INVALID_,
};

char const *stateNames[(size_t)States::_SIMPLE_FSM_INVALID_ + 1] = {
  "ON",
  "OFF",
  "INVALID"
};

enum class Events {
  TOGGLE
};

char const *eventNames[1] = {
  "TOGGLE"
};

using namespace SimpleFSM;
int main() {
  using FSM = HookableFSM<FSM<States, Events>, States, Events>;
  using LambdaState = LambdaState<States, Events>;
  FSM fsm;

  fsm.addState(new LambdaState(States::ON, {
    .entry = LambdaState::EntryFunction(),
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::OFF); break;
      }
    }
  }));

  fsm.addState(new LambdaState(States::OFF, {
    .entry = LambdaState::EntryFunction(),
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::ON); break;
      }
    }
  }));

  fsm.onTransition([](States from, States to) {
    std::cout << "Transitioned from state " << stateNames[(size_t)from] << " to state " << stateNames[(size_t)to] << std::endl;
  });

  fsm.onEvent([](Events event, FSM::EventPayload const &) {
    std::cout << "Received event " << eventNames[(size_t)event] << std::endl;
  });

  fsm.start(States::ON);
  fsm.emit(Events::TOGGLE);
  fsm.emit(Events::TOGGLE);
}

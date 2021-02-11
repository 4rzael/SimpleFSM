#include <iostream>
#include "SimpleFsm.hpp"

enum class States {
  ON,
  OFF,
  _SIMPLE_FSM_INVALID_,
};

enum class Events {
  TOGGLE
};

using namespace SimpleFSM;
int main() {
  using FSM = FSM<States, Events>;
  FSM fsm;

  fsm.addState(new FSM::LambdaState(States::ON, {
    .entry = []() {
      std::cout << "Entering state ON" << std::endl;
    },
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::OFF); break;
      }
    }
  }));

  fsm.addState(new FSM::LambdaState(States::OFF, {
    .entry = []() {
      std::cout << "Entering state OFF" << std::endl;
    },
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::ON); break;
      }
    }
  }));

  fsm.start(States::ON);
  fsm.emit(Events::TOGGLE);
  fsm.emit(Events::TOGGLE);
}

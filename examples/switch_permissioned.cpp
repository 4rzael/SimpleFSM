#include <iostream>
#include <cstdlib>
#include "SimpleFSM.hpp"
#include "LambdaState.hpp"
#include "PermissionedFSM.hpp"

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
  srand(0);

  using FSM = PermissionedFSM<FSM<States, Events>, States>;
  using LambdaState = LambdaState<States, Events>;
  FSM fsm;

  fsm.addState(new LambdaState(States::ON, {
    .entry = []() {
      std::cout << "Entering state ON" << std::endl;
    },
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::OFF); break;
      }
    }
  }));

  fsm.addState(new LambdaState(States::OFF, {
    .entry = []() {
      std::cout << "Entering state OFF" << std::endl;
    },
    .react = [&fsm](Events ev, FSM::EventPayload) {
      switch (ev) {
        case Events::TOGGLE: fsm.transit(States::ON); break;
      }
    }
  }));

  fsm.addRule([&fsm](States state) {
    return (rand() % 2 == 1) ? state : fsm.getCurrentState();
  });
  fsm.onPermissionRejection([](States to) {
    std::cout << "Permission rejected" << std::endl;
  });

  fsm.start(States::ON);
  std::cout << "Toggling" << std::endl;
  fsm.emit(Events::TOGGLE);
  std::cout << "Toggling" << std::endl;
  fsm.emit(Events::TOGGLE);
  std::cout << "Toggling" << std::endl;
  fsm.emit(Events::TOGGLE);
  std::cout << "Toggling" << std::endl;
  fsm.emit(Events::TOGGLE);
  std::cout << "Toggling" << std::endl;
  fsm.emit(Events::TOGGLE);
}

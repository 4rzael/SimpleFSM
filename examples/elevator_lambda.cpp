#include <iostream>
#include "SimpleFSM.hpp"

enum class States {
  MOVING,
  STOPPED,
  _SIMPLE_FSM_INVALID_,
};

enum class Events {
  CALL,
  OPEN_DOORS,
  FLOOR_SENSOR_HIT
};

using namespace SimpleFSM;
int main() {
  using FSM = FSM<States, Events, int>;
  FSM fsm;

  fsm.addState(new FSM::LambdaState(States::MOVING, {
    .entry = []() {
      std::cout << "Starting to move" << std::endl;
    },
    .react = [&fsm](Events ev, int floor) {
      switch (ev) {
        case Events::CALL:
          std::cout << "Cannot schedule a call to floor " << floor << ", already moving" << std::endl;
        break;
        case Events::OPEN_DOORS:
          std::cout << "Cannot open doors while moving" << std::endl;
        break;
        case Events::FLOOR_SENSOR_HIT:
          std::cout << "Destination reached" << std::endl;
          fsm.transit(States::STOPPED);
        break;
      }
    }
  }));

  fsm.addState(new FSM::LambdaState(States::STOPPED, {
    .entry = []() {
      std::cout << "Stopped" << std::endl;
    },
    .react = [&fsm](Events ev, int floor) {
      switch (ev) {
        case Events::CALL:
          std::cout << "Moving to floor " << floor << std::endl;
          fsm.transit(States::MOVING);
        break;
        case Events::OPEN_DOORS:
          std::cout << "Opening doors" << std::endl;
        break;
        default:
        break;
      }
    }
  }));

  constexpr int UNUSED = 0;

  fsm.start(States::STOPPED);
  fsm.emit(Events::CALL, 2);
  fsm.emit(Events::OPEN_DOORS, UNUSED);
  fsm.emit(Events::FLOOR_SENSOR_HIT, UNUSED);
  fsm.emit(Events::OPEN_DOORS, UNUSED);
  fsm.emit(Events::CALL, 1);
  fsm.emit(Events::FLOOR_SENSOR_HIT, UNUSED);
}

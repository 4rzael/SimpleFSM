#include <iostream>
#include "SimpleFSM.hpp"

enum class States {
  ON,
  OFF,
  _SIMPLE_FSM_INVALID_,
};

enum class Events {
  TOGGLE
};

using namespace SimpleFSM;
using SwitchFSM = FSM<States, Events>;

class BaseState : public virtual SwitchFSM::State {
  virtual void loop() {
    std::cout << "Looping" << std::endl;
  }
};

class OnState : public BaseState {
public:
  OnState(SwitchFSM &fsm): SwitchFSM::State(States::ON), _fsm(fsm) {}
  virtual void entry() {
    std::cout << "Entering state ON" << std::endl;
  }
  virtual void react(Events ev, SwitchFSM::EventPayload const &) {
    switch (ev) {
      case Events::TOGGLE: _fsm.transit(States::OFF); break;
    }
  }
  virtual void exit() {}

private:
  SwitchFSM &_fsm;
};

class OffState : public BaseState {
public:
  OffState(SwitchFSM &fsm): SwitchFSM::State(States::OFF), _fsm(fsm) {}
  virtual void entry() {
    std::cout << "Entering state OFF" << std::endl;
  }
  virtual void react(Events ev, SwitchFSM::EventPayload const &) {
    switch (ev) {
      case Events::TOGGLE: _fsm.transit(States::ON); break;
    }
  }
  virtual void exit() {}

private:
  SwitchFSM &_fsm;
};

int main() {
  SwitchFSM fsm;
  fsm.addState(new OnState(fsm));
  fsm.addState(new OffState(fsm));

  fsm.start(States::ON);
  fsm.update();
  fsm.emit(Events::TOGGLE);
  fsm.update();
  fsm.emit(Events::TOGGLE);
  fsm.update();
}

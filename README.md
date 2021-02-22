# SimpleFSM
[![CodeFactor](https://www.codefactor.io/repository/github/4rzael/simplefsm/badge)](https://www.codefactor.io/repository/github/4rzael/simplefsm)

A simple FSM header-only library in C++

## Why this library

* I didn't find an FSM library that was very simple to use, with a low amount of boilerplate

## Features

* FSMs with entry, loop and exit state methods

* Event handling

* Extendability by inheriting the FSM class

* (Optional) Either one class per state, or lambda-funtion based states

* (Optional) Transition, events & failure hooks

* (Optional) Runtime Allowed / Forbidden states & transitions

## Limitations

* The FSM is not polymorphism-compatible, as I didn't manage to get a virtual / static-asserted conditional no-payload emit()

## Features I want to add

* .dot file export

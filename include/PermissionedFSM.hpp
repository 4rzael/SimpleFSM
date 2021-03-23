#pragma once
#include <SimpleFSM.hpp>
#include <vector>

#ifndef SIMPLE_FSM_MAX_RULES_RESERVED // Defines how many rules we reserve space for before having to use reallocs
# define SIMPLE_FSM_MAX_RULES_RESERVED 8
#endif

namespace SimpleFSM {
  template <class Base, class StateEnum>
  class PermissionedFSM : public Base {
    public:
    PermissionedFSM() {
      _rules.reserve(SIMPLE_FSM_MAX_RULES_RESERVED);
    }

    using Rule                        = std::function<StateEnum (StateEnum state)>;
    using PermissionRejectedCallback  = std::function<void (StateEnum to)>;

    void addRule(Rule rule) {
      _rules.push_back(rule);
    }

    void onPermissionRejection(PermissionRejectedCallback cb) {
      _rejectCallbacks.push_back(cb);
    }

    FSMError transit(StateEnum newState) {
      auto redirect = _checkForPermission(newState);
      if (redirect != newState) {
        _forceTransit(redirect);
        return FSMError::INVALID_PERMISSION;
      }
      return Base::transit(newState);
    }

    FSMError update() {
      auto state = Base::getCurrentState();
      auto redirect = _checkForPermission(state);
      if (state != redirect) {
        _forceTransit(redirect);
        return FSMError::INVALID_PERMISSION;
      }
      return Base::update();
    }

    FSMError checkRules() {
      auto state = Base::getCurrentState();
      auto redirect = _checkForPermission(state);
      if (state != redirect) {
        _forceTransit(redirect);
        return FSMError::INVALID_PERMISSION;
      }
      return FSMError::OK;
    }

    bool wouldAllowState(StateEnum state) {
      return _checkForPermission(state) == state;
    }

  private:
    FSMError _forceTransit(StateEnum state) {
      return Base::transit(state);
    }

    StateEnum _checkForPermission(StateEnum newState) {
      for (auto &rule : _rules) {
        auto redirect = rule(newState);
        if (redirect != newState) {
          for (auto &cb : _rejectCallbacks) {
            cb(newState);
          }
          return redirect;
        }
      }
      return newState;
    }

      std::vector<Rule>                       _rules;
      std::vector<PermissionRejectedCallback> _rejectCallbacks;
  };
};

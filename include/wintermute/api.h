#pragma once

#include "pros/rtos.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace wintermute {

using std::string;

class State {

public:
	virtual ~State() {
	}
	virtual string execute() = 0;
};

class StateMachine {

	std::unordered_set<string> terminals;
	std::unordered_map<string, State*> states;
	std::unordered_map<string, string> transitions;

public:
	StateMachine(std::unordered_set<string> terminals) : terminals(terminals) {
		this->states = {};
		this->transitions = {};
	}

	void add_transition(string state_name, State& state,
	                    std::unordered_map<string, string> state_transitions) {
		this->states[state_name] = &state;
		for (const auto& [key, value] : state_transitions) {
			string new_key = state_name + " " + key;
			this->transitions[new_key] = value;
		}
	}

	void execute(string initial_state) {
		string current_name = initial_state;
		State* current_state = this->states[initial_state];

		while (true) {
			string outcome = current_state->execute();
			current_name = this->transitions[current_name + " " + outcome];
			current_state = this->states[current_name];

			if (terminals.find(current_name) != terminals.end()) {
				break;
			}

			pros::delay(10);
		}
	}
};

} // namespace wintermute
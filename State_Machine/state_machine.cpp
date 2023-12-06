#include <array>
#include <iostream>
#include <string>
#include "state.h"
#include "state_machine.h"

void State_Machine::set_next_state(Abstract_State& state) {
    next_state = state;
}

void State_Container::entry_behavior() override {
    state::entry_behavior(input, output);
}
void State_Container::do_behavior() override {
    state::do_behavior(input, output);
}
void State_Container::exit_behavior() override {
    state::exit_behavior(input, output);
}
State& State_Container::get_next_state() override {
    return state::get_next_state(input);
}
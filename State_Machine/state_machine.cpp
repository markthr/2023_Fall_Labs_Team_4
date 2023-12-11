#ifndef __STATE_MACHINE_C__
#define __STATE_MACHINE_C__

#include <string>
#include "Arduino.h"
#include "state_machine.hpp"


template<typename I, typename O>
void State_Machine<I, O>::iterate() {
    this->current_state->do_behavior(input, output);

    Abstract_State<I, O>* next_state = &this->current_state->get_next_state(input);

    if(*next_state != *current_state) {
        Serial.print("Transition: ");
        Serial.print(this->current_state->name.c_str());
        Serial.print(" -> ");
        Serial.println(next_state->name.c_str());
        this->current_state->exit_behavior(this->input, this->output);
        this->current_state = next_state;
        this->current_state->entry_behavior(this->input, this->output);
    }

}

template<typename I, typename O>
std::string State_Machine<I, O>::get_state_name() {
    return this->current_state->name;
}

template<typename I, typename O>
int State_Machine<I, O>::get_state_code() {
    return this->current_state->state_code;
}

template<typename I, typename O>
bool operator==(const Abstract_State<I, O>& a, const Abstract_State<I, O>& b) {
    return a.state_code == b.state_code; // assume state_code is unique
}

template<typename I, typename O>
bool operator!=(const Abstract_State<I, O>& a, const Abstract_State<I, O>& b) {
    return !(a == b); // ensure that == and != are boolean opposites
}

#endif
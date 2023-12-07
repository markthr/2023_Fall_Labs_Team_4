#ifndef __STATE_MACHINE_C__
#define __STATE_MACHINE_C__

#include <string>
#include "state_machine.hpp"


template<typename I, typename O>
void State_Machine<I, O>::iterate() {
    current_state.do_behavior(input, output);

    Abstract_State<I, O>& next_state = current_state.get_next_state(input);

    if(next_state != current_state) {
        current_state.exit_behavior(input, output);
        current_state = next_state;
    }

}

template<typename I, typename O>
std::string State_Machine<I, O>::get_state_name() {
    return current_state.name;
}

template<typename I, typename O>
bool operator==(const Abstract_State<I, O>& a, const Abstract_State<I, O>& b) {
    return a.name == b.name; // assume names are unique
}

template<typename I, typename O>
bool operator!=(const Abstract_State<I, O>& a, const Abstract_State<I, O>& b) {
    return !(*a == *b); // ensure that == and != are boolean opposites
}

#endif
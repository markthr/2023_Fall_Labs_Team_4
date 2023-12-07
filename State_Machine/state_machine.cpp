#include <array>
#include <iostream>
#include <string>
#include "state_machine.h"

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
void State_Machine<I, O>::operator==(const State_Machine<I, O>& other) {
    return name == other.name; // assume names are unique
}

template<typename I, typename O>
void State_Machine<I, O>::operator==(const State_Machine<I, O>& other) {
    return !(*this == other); // ensure that == and != are boolean opposites
}
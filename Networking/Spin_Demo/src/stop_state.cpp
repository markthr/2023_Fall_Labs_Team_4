#include "sd_state_machine.hpp"

Stop_State& Stop_State::instance() {
    static Stop_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Stop_State::get_next_state(const Input& input) {
    return Rest_State::instance();
}

void Stop_State::entry_behavior(const Input& input, Output& output) {
    // TODO: turn on brake
}

void Stop_State::do_behavior(const Input& input, Output& output) {
    output.screen.drawString(0, 6, "State: Stop");
}

void Stop_State::exit_behavior(const Input& input, Output& output) {
    // TODO: turn off brake
}
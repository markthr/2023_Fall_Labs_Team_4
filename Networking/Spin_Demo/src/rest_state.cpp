#include "sd_state_machine.hpp"

Rest_State& Rest_State::instance() {
    static Rest_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Rest_State::get_next_state(const Input& input) {
    return Rest_State::instance();
}

void Rest_State::do_behavior(const Input& input, Output& output) {
  output.screen.drawString(0, 6, "State: Rest");
}
#include "sd_state_machine.hpp"

Startup_State& Startup_State::instance() {
    static Startup_State __instance;
    return __instance;
}

void Startup_State::entry_behavior(const Input& input, Output& output) {
    output.screen.clearDisplay();
    output.screen.drawString(0, 6, "State: Startup");
}
void Startup_State::do_behavior(const Input& input, Output& output) {
    // do nothing
}

Abstract_State<Input, Output>& Startup_State::get_next_state(const Input& input) {
    // wait 3 seconds for demo purposes
    if(input.gyro.timestamp <= 3000) {
        return Startup_State::instance();
    }
    else {
        return Rest_State::instance();
    }
}
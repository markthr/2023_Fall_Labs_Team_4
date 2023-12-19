#include "sd_state_machine.hpp"

Stop_State& Stop_State::instance() {
    static Stop_State __instance;
    return __instance;
}

/**
 * Remain in stop state for the set duration and then transition to rest
*/
Abstract_State<Input, Output>& Stop_State::get_next_state(const Input& input) {
    if(input.gyro.timestamp - this->entry_time >= STOP_STATE_LEN_MILLIS) {
        return Rest_State::instance();
    }
    else {
        return Stop_State::instance();
    }
    
}

/**
 * turn on brake when entering the state
*/
void Stop_State::entry_behavior(const Input& input, Output& output) {
    output.screen.clearDisplay();
    output.screen.drawString(0, 6, "State: Stop");
    
    this->entry_time = input.gyro.timestamp;

    output.left_motor.brake();
    output.right_motor.brake();
}

void Stop_State::do_behavior(const Input& input, Output& output) {
    // do nothing
}

/**
 * turn of the brake when leaving the state
*/
void Stop_State::exit_behavior(const Input& input, Output& output) {
    output.left_motor.stop();
    output.right_motor.stop();
}
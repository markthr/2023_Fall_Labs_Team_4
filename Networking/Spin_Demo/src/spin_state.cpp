#include "sd_state_machine.hpp"
#include <cstdlib>

#define ROTATION_TOLERANCE 2

Spin_State& Spin_State::instance() {
    static Spin_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Spin_State::get_next_state(const Input& input) {
    if(input.receive_packet.next_state == STOP_STATE_CODE) {
        return Stop_State::instance(); // stop if commanded to
    }
    else if(std::abs(this->rotation - 360) < ROTATION_TOLERANCE) {
        return Stop_State::instance(); // stop if finished
    }
    else {
        return Spin_State::instance(); // continue spinning if not done
    }
}

void Spin_State::entry_behavior(const Input& input, Output& output) {
    this->rotation = 0; // reset the integral
    this->prev_timestamp = input.gyro.timestamp; // update the initial time so the error from the first step isnt too big
}

void Spin_State::do_behavior(const Input& input, Output& output) {
    output.screen.drawString(0, 6, "State: Spin");
    // TODO: actually make it spin
}
#include "sd_state_machine.hpp"
#include <cstdlib>
#include <stdio.h>



Spin_State& Spin_State::instance() {
    static Spin_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Spin_State::get_next_state(const Input& input) {
    if(input.receive_packet.next_state == STOP_STATE_CODE) {
        return Stop_State::instance(); // stop if commanded to
    }
    else if(std::abs(this->rotation - 360) < SPIN_ROTATION_TOLERANCE) {
        return Stop_State::instance(); // stop if finished
    }
    else {
        return Spin_State::instance(); // continue spinning if not done
    }
}

void Spin_State::entry_behavior(const Input& input, Output& output) {
    output.screen.clearDisplay();
    output.screen.drawString(0, 6, "State: Spin");

    // reset the integral
    this->rotation = 0; 
    this->integral_error = 0;
    // update the initial timestamp to not have a massive timestep at the beginning
    this->prev_timestamp = input.gyro.timestamp; 
}

void Spin_State::do_behavior(const Input& input, Output& output) {
    
    
    int dt = input.gyro.timestamp - this->prev_timestamp;
    int error = 360 - this->rotation;
    this->integral_error += error * 0.001;
    // update rotation integral, factor of 0.057296 is conversion from rad/s to deg/ms
    this->rotation += input.gyro.gyro.z * dt * 0.057296f; 

    // get pwm value to write, proportional control with bias where 0.355f corresponds to 128/360
    float pwm = error*0.355f + this->integral_error * 0.355f;

    // if pwm is positive then need to rotate left, if pwm is negative then need to rotate right
    output.left_motor.set_pwm(-pwm);
    output.right_motor.set_pwm(pwm);

    this->prev_timestamp = input.gyro.timestamp;

    snprintf(output.send_packet.state_log, STATE_LOG_LEN, "Rotation: %f (deg), Integral Error: %f (deg) dt: %d (ms)",
            this->rotation, this->integral_error, dt);
}
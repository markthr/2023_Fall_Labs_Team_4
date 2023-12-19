#include "sd_state_machine.hpp"
#define BALL_STATE_DRIVE_TIMEOUT_MILLIS 1000
#define BALL_STATE_SEARCH_TIMEOUT_MILLIS 10000
#define BALL_STATE_VERTICAL_THRESHOLD 0.2
#define BALL_STATE_CHASE_SPEED 64

const float BALL_K_P = 20;
const float BALL_K_I = 2;
const float BALL_K_D = 2;
const float BALL_I_ERROR_MAX = (255-BALL_STATE_CHASE_SPEED) / BALL_K_I;

Ball_State& Ball_State::instance() {
    static Ball_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Ball_State::get_next_state(const Input& input) {
    if(input.receive_packet.next_state == STOP_STATE_CODE) {
        // commanded to stop, so stop
        return Stop_State::instance();
    }
    else if(input.gyro.timestamp - this->prev_timestamp >= BALL_STATE_SEARCH_TIMEOUT_MILLIS) {
        // lost the ball (or comms), stop
        return Stop_State::instance();
    }
    else {
        // know where the ball is and no command, keep following it
        return Ball_State::instance();
    }
}

void Ball_State::entry_behavior(const Input& input, Output& output) {
    output.screen.clearDisplay();
    output.screen.drawString(0, 6, "State: Ball");

    this->integral_error = 0;
    this->derivative_error = 0;
    this->prev_timestamp = input.gyro.timestamp;
}
void Ball_State::do_behavior(const Input& input, Output& output) {
    float u; // value to determine what is written to motors

    if((!input.receive_packet.ball_detected || input.receive_packet.stale) 
        && input.gyro.timestamp - prev_timestamp  > BALL_STATE_DRIVE_TIMEOUT_MILLIS) {
        // ball not detected timeout, stop driving
        this->integral_error = 0;
        this->derivative_error = 0;
        this->timeout = true;

        u = 0;
    }
    else if(timeout) {
        // a timeout just ended, set the initial conditions
        this->prev_offset_x = input.receive_packet.ball_offset_x;
        this->prev_timestamp = input.receive_packet.timestamp;

        u = 0;
    }
    else if(input.receive_packet.ball_detected) {
        // the ball is detected, track it
        float dt = this->prev_timestamp - input.receive_packet.timestamp;
        float dx = this->prev_offset_x - input.receive_packet.ball_offset_x;

        // inegral with saturation
        this->integral_error += (-input.receive_packet.ball_offset_x) * dt;
        if(this->integral_error > BALL_I_ERROR_MAX) {
            this->integral_error = BALL_I_ERROR_MAX;
        }
        else if(this->integral_error < -BALL_I_ERROR_MAX) {
            this->integral_error = -BALL_I_ERROR_MAX;
        }

        this->derivative_error = dx/dt; // simple derivative
        this->prev_offset_x = prev_offset_x; // update prev
        u = BALL_K_P * dx + BALL_K_I * this->integral_error + BALL_K_D * this->derivative_error;

        if(input.receive_packet.ball_offset_y < BALL_STATE_VERTICAL_THRESHOLD) {
            // ball low enough in the frame, chase
            output.left_motor.set_pwm(BALL_STATE_CHASE_SPEED  + u);
            output.right_motor.set_pwm(BALL_STATE_CHASE_SPEED - u);
        }
        else {
            // ball too high in the frame, just idly track
            output.left_motor.set_pwm(u);
            output.right_motor.set_pwm(-u);
        }
    }
}
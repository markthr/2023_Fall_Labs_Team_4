#include <Arduino.h>
#include <cmath>
#include "motors.hpp"


void Motor::init(uint32_t in1, uint32_t in2, uint32_t i_sense, uint32_t frequency, uint32_t resolution) {
    ledcSetup(this->in1_channel, frequency, resolution);
    ledcSetup(this->in2_channel, frequency, resolution);

    ledcAttachPin(in1, this->in1_channel);
    ledcAttachPin(in2, this->in2_channel);

    pinMode(i_sense, INPUT);

    // compute 2^resolution - 1
    this->pwm_max = (1<<resolution) - 1;
}

void Motor::brake() {
    ledcWrite(this->in1_channel, pwm_max);
    ledcWrite(this->in2_channel, pwm_max);
}

void Motor::stop() {
    ledcWrite(this->in1_channel, 0);
    ledcWrite(this->in2_channel, 0);
}

void Motor::set_pwm(int32_t duty){
    if(duty > 255) {
        duty = 255;
    }
    else if (duty < -255) {
        duty = -255;
    }

    if (duty > 0) {
        ledcWrite(this->in1_channel, 0);
        ledcWrite(this->in2_channel, (uint32_t)(duty));
    }
    else {
        ledcWrite(this->in1_channel, (uint32_t)-duty);
        ledcWrite(this->in2_channel, 0);
    }
}

void Motor::set_pwm(float duty){
    if (isnan(duty)) {
        duty = 0.0;
    }
    else if (duty > 255.0) {
        duty  =  255.0;
    }
    else if (duty < -255) {
        duty = -255;
    }

    if (duty > 0) {
        ledcWrite(this->in1_channel, 0);
        ledcWrite(this->in2_channel, (uint32_t)(duty));
    }
    else {
        ledcWrite(this->in1_channel, (uint32_t)-duty);
        ledcWrite(this->in2_channel, 0);
    }
}


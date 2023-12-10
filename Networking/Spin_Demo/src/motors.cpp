#include <Arduino.h>
#include "motors.hpp"


void setup_motors() {
    ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
    ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
    ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
    ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

    ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
    ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
    ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
    ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

    pinMode(M1_I_SENSE, INPUT);
    pinMode(M2_I_SENSE, INPUT);
}

void forward(struct Motor* m, uint32_t duty){
  ledcWrite(m->in1, 0);
  ledcWrite(m->in2, duty);
}

void backward(struct Motor* m, uint32_t duty){
  ledcWrite(m->in1, duty);
  ledcWrite(m->in2, 0);
}

void brake(struct Motor* m) {
  ledcWrite(m->in1, 1023);
  ledcWrite(m->in2, 1023);
}
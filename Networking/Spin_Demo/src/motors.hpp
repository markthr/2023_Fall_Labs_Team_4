#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>

const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 0;
const unsigned int M1_IN_2_CHANNEL = 1;
const unsigned int M2_IN_1_CHANNEL = 2;
const unsigned int M2_IN_2_CHANNEL = 3;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const int freq = 5000;
const int resolution = 10;


struct Motor {
  const unsigned int in1;
  const unsigned int in2;

  Motor(int in1, int in2) : in1(in1), in2(in2) {}
};

void forward(struct Motor* m, uint32_t duty);
void backward(struct Motor* m, uint32_t duty);
void brake(struct Motor* m);
void setup_motors();

#endif

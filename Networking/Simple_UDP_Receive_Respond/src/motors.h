#ifndef __MOTORS_H__
#define __MOTORS_H__

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


struct motor{
  const unsigned int in1;
  const unsigned int in2;
};

void forward(struct motor* m, uint32_t duty);
void backward(struct motor* m, uint32_t duty);
void brake(struct motor* m);
void setup_motors();

#endif

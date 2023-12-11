#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>

#define MOTOR_DEFAULT_FREQUENCY 5000
#define MOTOR_DEFAULT_RESOLUTION 8

#define M1_IN_1_CHANNEL 8
#define M1_IN_2_CHANNEL 9
#define M2_IN_1_CHANNEL 10
#define M2_IN_2_CHANNEL 11

const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;



class Motor {
    public:
        Motor(uint32_t in1_channel, uint32_t in2_channel)
        : in1_channel(in1_channel), in2_channel(in2_channel), pwm_max(0) {}

    private:
        const uint32_t in1_channel;
        const uint32_t in2_channel;
        uint32_t pwm_max;

    public:
        /**
         * Configure the IO pins to initialize the motor
        */
        void init(uint32_t in1, uint32_t in2, uint32_t i_sense,
            uint32_t frequency = MOTOR_DEFAULT_FREQUENCY, uint32_t resolution = MOTOR_DEFAULT_RESOLUTION);

        /**
         * Short the H-bridge to stop
         * 
         * Calling before init(...) causes undefined behavior
        */
        void brake();

        /**
         * Set the PWM to 0
        */
        void stop();

        /**
         * Convert the float to a uint32_t and write that value
         * Duty magnitude above 2^resolution-1 makes no difference 
         * Negative values indicate reverse
         * NaN is replaced with 0
         * 
         * Calling before init(...) causes undefined behavior
        */
        void set_pwm(float duty);

        /**
         * Convert the float to a uint32_t and write that value
         * Duty magnitude above 2^resolution-1 makes no difference 
         * Negative values indicate reverse
         * 
         * Calling before init(...) causes undefined behavior
        */
        void set_pwm(int32_t duty);
};

#endif

#ifndef __SD_STATE_MACHINE_H__
#define __SD_STATE_MACHINE_H__

#include <Arduino.h>
#include <string>
#include <U8x8lib.h>
// #include <Encoder.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_MPU6050.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "state_machine.hpp"
#include "state_machine.cpp"
#include "motors.hpp"
#include "udp_packets.hpp"

#define STARTUP_STATE_CODE 0
#define REST_STATE_CODE 1
#define STOP_STATE_CODE 2
#define SPIN_STATE_CODE 3
#define BALL_STATE_CODE 4

#define STOP_STATE_LEN_MILLIS 200
#define SPIN_ROTATION_TOLERANCE 2

struct Encoder {

};

struct Input {
  Encoder& left;
  Encoder& right;
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  Receive_Packet& receive_packet;

  Input(Encoder& left, Encoder& right, Receive_Packet& receive_packet) 
  : left(left), right(right), receive_packet(receive_packet) {}
};

struct Output {
  Motor& left_motor;
  Motor& right_motor;
  U8X8_SSD1306_128X64_NONAME_SW_I2C& screen;
  Send_Packet& send_packet;

  Output(Motor& left_motor, Motor& right_motor, U8X8_SSD1306_128X64_NONAME_SW_I2C& screen, Send_Packet& send_packet)
  : left_motor(left_motor), right_motor(right_motor), screen(screen), send_packet(send_packet) {}
};

class Startup_State : public Abstract_State<Input, Output> {
    public:
        static Startup_State& instance();

        void do_behavior(const Input& input, Output& output) override;
        void entry_behavior(const Input& input, Output& output) override;
        Abstract_State<Input, Output>& get_next_state(const Input& input) override;

    private:
        Startup_State()
        : Abstract_State("Startup", STARTUP_STATE_CODE) {}
};

class Rest_State : public Abstract_State<Input, Output> {
    public:
        static Rest_State& instance();
        
        void do_behavior(const Input& input, Output& output) override;
        void entry_behavior(const Input& input, Output& output) override;
        Abstract_State<Input, Output>& get_next_state(const Input& input) override;

    private:
        Rest_State()
        : Abstract_State("Rest", REST_STATE_CODE) {}
};

class Stop_State : public Abstract_State<Input, Output> {
    private:
        unsigned long entry_time;
    public:
        static Stop_State& instance();
        
        void entry_behavior(const Input& input, Output& output) override;
        void do_behavior(const Input& input, Output& output) override;
        void exit_behavior(const Input& input, Output& output) override;
        Abstract_State<Input, Output>& get_next_state(const Input& input) override;

    private:
        Stop_State()
        : Abstract_State("Stop", STOP_STATE_CODE) {}
};

class Spin_State : public Abstract_State<Input, Output> {
    private:
        float rotation;
        float integral_error;
        long prev_timestamp;
    public:
        static Spin_State& instance();

        void entry_behavior(const Input& input, Output& output) override;
        void do_behavior(const Input& input, Output& output) override;
        Abstract_State<Input, Output>& get_next_state(const Input& input) override;

    private:
        Spin_State()
        : Abstract_State("Spin", SPIN_STATE_CODE) {}
};

class Ball_State : public Abstract_State<Input, Output> {
    private:
        float integral_error;
        float derivative_error;
        float prev_offset_x;
        bool timeout;
        long prev_timestamp;

    public:
        static Ball_State& instance();

        void entry_behavior(const Input& input, Output& output) override;
        void do_behavior(const Input& input, Output& output) override;
        Abstract_State<Input, Output>& get_next_state(const Input& input) override;

    private:
        Ball_State()
        : Abstract_State("Ball State", BALL_STATE_CODE) {}
};

#endif
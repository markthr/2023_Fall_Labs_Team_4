#include <Arduino.h>
#include <string>
#include "state_machine.h"


struct Receive_Packet {
  unsigned int timestamp;
  unsigned int next_state;
};

struct Send_Packet {
  unsigned int current_state;
  float angular_acc_x;
  float angular_acc_y;
  float angular_acc_z;
  char state_log[64];
};

class Motor {

};

class IMU {

};

class Encoder {

};

class U8x8 {

};

struct Input {
  Receive_Packet* receive_packet;
  Encoder* left;
  Encoder* right;
  IMU* imu;
};

struct Output {
  const Motor* left;
  const Motor* right;
  const U8x8* screen;
  char log[64];
};

class Rest_State : public Abstract_State<Input, Output> {
    public:
        static Rest_State& instance();
        
        
        Rest_State& get_next_state(const Input& input);

    private:
        Rest_State()
        : Abstract_State("Startup") {}
};
class Startup_State : public Abstract_State<Input, Output> {
    public:
        static Startup_State& instance() {
            static Startup_State __instance;
            return __instance;
        }
        
        
        Abstract_State& get_next_state(const Input& input) override {
            return Rest_State::instance();
        }


    private:
        Startup_State()
        : Abstract_State("Startup") {}
};

Rest_State& Rest_State::instance() {
    static Rest_State __instance;
    return __instance;
}
        
        
Rest_State& Rest_State::get_next_state(const Input& input) {
    return Rest_State::instance();
}

Input input;
Output output;
State_Machine<Input, Output> sm(Startup_State::instance(), input, output);

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

Send_Packet send_packet;



void loop() {
  sm.iterate();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
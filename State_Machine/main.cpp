#include <array>
#include <iostream>
#include <string>
#include "state_machine.h"


// placeholder for actual input and output
struct Output {
    std::string str;
};

struct Input {
    std::string str;
};

class Startup_State : public Abstract_State<Input, Output> {
    public:
        static Startup_State& instance() {
            static Startup_State __instance;
            return __instance;
        }
        
        
        Abstract_State& get_next_state(const Input& input) override {
            return Startup_State::instance();
        }


    private:
        Startup_State()
        : Abstract_State("Startup") {}
    // Startup_State(Startup_State const&); // Don't Implement
    // void operator=(Startup_State const&); // Don't implement
    // public:
    //     Startup_State(Startup_State const&) = delete;
    //     void operator=(Startup_State const&) = delete;
};

Input input = {"input"};
Output output = {"output"};




void setup() {
    State_Machine<Input, Output> sm(Startup_State::instance(), input, output);
}






int main() {
    setup();


}



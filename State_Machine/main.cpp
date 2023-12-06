#include <array>
#include <iostream>
#include <string>
#include "state.h"
#include "state_machine.h"

/*
 * Don't overengineer. No need to program for the most general case. Just the case presented in class.
 *
 * Known properties of the course:
 * There are no gaps in a branch. Therefore a line dissapearing means that exploration of the branch is complete.
 * Branches do not themselves have branches. No need to have a stack to push arbitrary numbers of branches.
 * Detection of object of interest can only occur during branches and only at the end of the branch.
 * 
 */
// use std::array for safety
// inner array indexes the return values for a given State_Code. 
// outer array indexes the States

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
        
        
        Abstract_State& get_next_state(Input& input) override {
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
class State_Machine {
    public: 
        explicit State_Machine() {
            current_state = new State_Container<Input, Output>(Startup_State::instance(), input, output);
        }

    private:
        State* current_state;
};


void setup() {
    State_Machine sm;
}






int main() {
    setup();


}



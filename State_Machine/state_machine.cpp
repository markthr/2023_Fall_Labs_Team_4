#include <array>
#include <iostream>
#include<string>
#include "state.h"

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
std::array<std::array<State_Code, (int)Return_Value::_Count>, (int)State_Code::_Count> transitions;

// placeholder for actual input and output
struct Output {
    std::string str;
};

struct Input {
    std::string str;
};

class State: public Abstract_State<Input, Output> {
    public:
        using Abstract_State<Input, Output>::Abstract_State;
        Return_Value guard(Input &input) override {
            std::cout << "Input: " << input.str << std::endl;
            return Return_Value::Complete;
        }

        void do_behavior(Input &input, Output &output) override {
            std::cout << "Old Output: " << output.str << std::endl;
            output.str = "The new one";
        }
};

State state = State(State_Code::Startup);

void setup() {
    // make it so garbage stops the bot
    for(int i=0; i<(int)State_Code::_Count; i++) {
        for(int j=0; j<(int)Return_Value::_Count; j++) {
            transitions[i][j] = State_Code::Stop;
        }
    }
    

    // make it so loop has the State_Code machine stay in the same State_Code
    for(int i=0; i<(int)State_Code::_Count; i++) {
        transitions[i][(int)Return_Value::Loop] = static_cast<State_Code>((size_t)i+1);
    }
    
    // Startup Transitions
    transitions[(int)State_Code::Startup][(int)Return_Value::Timeout] = State_Code::Forward;

    // Forward Transitions
    transitions[(int)State_Code::Forward][(int)Return_Value::Line_Loss] = State_Code::Gap;
    transitions[(int)State_Code::Forward][(int)Return_Value::Intersection] = State_Code::Branch;

    // Branch Transitions
    transitions[(int)State_Code::Branch][(int)Return_Value::Complete] = State_Code::Detection;
    transitions[(int)State_Code::Branch][(int)Return_Value::Line_Loss] = State_Code::Backtrack;
    transitions[(int)State_Code::Branch][(int)Return_Value::Intersection] = State_Code::Forward;

    // Backtrack Transitions
    transitions[(int)State_Code::Backtrack][(int)Return_Value::Complete] = State_Code::Forward;

    // Gap Transitions
    transitions[(int)State_Code::Gap][(int)Return_Value::Timeout] = State_Code::Stop;
    transitions[(int)State_Code::Gap][(int)Return_Value::Complete] = State_Code::Forward;

    // Detection Transitions
    transitions[(int)State_Code::Detection][(int)Return_Value::Timeout] = State_Code::Backtrack;
}






int main() {
    setup();

    Input input = {"This is a nice input"};
    Output output = {"The old one"};


    std::cout << "State: " << NAME(State_Code::Forward) << std::endl;
    state.do_behavior(input, output);
    state.guard(input);
    state.do_behavior(input, output);
}



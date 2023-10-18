#include <array>
#include <iostream>

#define NAME(Variable) (#Variable)

/**
 * @brief 
 * Startup: the system has just been turned on
 * Forward: moving over the line
 * Gap: the line has ended, continue on for a bit to see if it is a dashed line
 * Detection: the object of interest has been detected at the end of a branching path
 * Stop: finished navigating
 */
enum class State {Startup, Forward, Backtrack, Gap, Detection, Stop, _Count};

int branches = 0; // if a branch is detected, increment. If a branch is fully explored decrement.
bool branch_right; // if turned right to follow branch true. If turned left to follow branch false

enum class Return_Value {Loop, Timeout, Complete, _Count};


// use std::array for safety
// inner array indexes the return values for a given state. 
// outer array indexes the states
std::array<std::array<State, (int)Return_Value::_Count>, (int)State::_Count> transitions;

void setup() {
    transitions[(int)State::Startup][(int)Return_Value::Timeout] = State::Forward;
    transitions[(int)State::Gap][(int)Return_Value::Timeout] = State::Stop;
    transitions[(int)State::Backtrack][(int)Return_Value::Complete] = State::Forward;
}

int main() {
    

    std::cout << "State: " << NAME(State::Forward) << std::endl;
}
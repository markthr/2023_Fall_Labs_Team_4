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
enum class State {Startup, Forward, Branch, Backtrack, Gap, Detection, Stop, _Count};

enum class Branch_Direction {Right, Left};



enum class Return_Value {Loop, Timeout, Intersection, Line_Loss, Complete, _Count};

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
// inner array indexes the return values for a given state. 
// outer array indexes the states
std::array<std::array<State, (int)Return_Value::_Count>, (int)State::_Count> transitions;

void setup() {
    // make it so garbage stops the bot
    for(int i=0; i<(int)State::_Count; i++) {
        for(int j=0; j<(int)Return_Value::_Count; j++) {
            transitions[i][j] = State::Stop;
        }
    }
    

    // make it so loop has the state machine stay in the same state
    for(int i=0; i<(int)State::_Count; i++) {
        transitions[i][(int)Return_Value::Loop] = static_cast<State>((size_t)i+1);
    }
    
    // Startup Transitions
    transitions[(int)State::Startup][(int)Return_Value::Timeout] = State::Forward;

    // Forward Transitions
    transitions[(int)State::Forward][(int)Return_Value::Line_Loss] = State::Gap;
    transitions[(int)State::Forward][(int)Return_Value::Intersection] = State::Branch;

    // Branch Transitions
    transitions[(int)State::Branch][(int)Return_Value::Complete] = State::Detection;
    transitions[(int)State::Branch][(int)Return_Value::Line_Loss] = State::Backtrack;
    transitions[(int)State::Branch][(int)Return_Value::Intersection] = State::Forward;

    // Backtrack Transitions
    transitions[(int)State::Backtrack][(int)Return_Value::Complete] = State::Forward;

    // Gap Transitions
    transitions[(int)State::Gap][(int)Return_Value::Timeout] = State::Stop;
    transitions[(int)State::Gap][(int)Return_Value::Complete] = State::Forward;

    // Detection Transitions
    transitions[(int)State::Detection][(int)Return_Value::Timeout] = State::Backtrack;
}


int main() {
    

    std::cout << "State: " << NAME(State::Forward) << std::endl;
}
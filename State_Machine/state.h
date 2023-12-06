#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include "state_machine.h"

#define NAME(Variable) (#Variable)

/**
 * @brief 
 * Startup: the system has just been turned on
 * Forward: moving over the line
 * Gap: the line has ended, continue on for a bit to see if it is a dashed line
 * Detection: the object of interest has been detected at the end of a branching path
 * Stop: finished navigating
 */

// TODO: ponder if this being in the namespace of State is nice, is this a good idea for return values too?


class State {
    public:
        virtual void entry_behavior() = 0;
        virtual void do_behavior() = 0;
        virtual void exit_behavior() = 0;
        virtual State& get_next_state() = 0;
        virtual ~State() {}
};

template<typename I, typename O>
class Abstract_State {
    public: 
        const std::string name;

        virtual void entry_behavior(I&, O&) {}
        virtual void do_behavior(I&, O&) {}
        virtual void exit_behavior(I&, O&) {}
        virtual Abstract_State& get_next_state(I&) = 0;
    
    protected:
        explicit Abstract_State(std::string name) : name(name){}


};

template<typename I, typename O>
class State_Container : public State {
    private:
        I& input;
        O& output;
        Abstract_State<I, O>& state;
    public:
        explicit State_Container(Abstract_State<I, O>& state, I& input, O& output) : state(state), input(input), output(output) {}
};;

#endif
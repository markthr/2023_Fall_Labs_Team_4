#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <array>
#include <iostream>
#include <string>

template<typename I, typename O>
class Abstract_State {
    public: 
        const std::string name; // name must be unique!

        virtual void entry_behavior(const I&, O&) {}
        virtual void do_behavior(const I&, O&) {}
        virtual void exit_behavior(const I&, O&) {}
        virtual Abstract_State<I, O>& get_next_state(const I&) = 0;
    
    protected:
        explicit Abstract_State(std::string name) : name(name){}


};

template<typename I, typename O>
class State_Machine {
    public: 
        explicit State_Machine(Abstract_State<I, O>& initial_state, I& input, O& output) : current_state(initial_state), input(input), output(output) {}

    private:
        Abstract_State<I, O>& current_state;
        I& input;
        O& output;
    
    public:
        void iterate();
};


#endif


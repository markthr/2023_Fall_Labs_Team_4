#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <array>
#include <iostream>
#include <string>
#include <functional>

template<typename I, typename O>
class Abstract_State {
    public: 
        const std::string name; // name must be unique!
        const int state_code; // state_code must be unique!

        virtual void entry_behavior(const I& input, O& output) {}
        virtual void do_behavior(const I& input, O& output) {}
        virtual void exit_behavior(const I& input, O& output) {}
        virtual Abstract_State<I, O>& get_next_state(const I& input) = 0;
    
    protected:
        explicit Abstract_State(const std::string name, int state_code) : name(name), state_code(state_code) {}
};

template<typename I, typename O>
class State_Machine {
    public: 
        explicit State_Machine(Abstract_State<I, O>& initial_state, I& input, O& output) : current_state(&initial_state), input(input), output(output) {}
        
    private:
        Abstract_State<I, O>* current_state; // needs to be a pointer to allow states to change
        
    protected:
        I& input;
        O& output;
    
    public:
        void iterate();
        std::string get_state_name();
        int get_state_code();

        /**
         * Optional function which will be executed before each iteration and processing of transitions
         * Intended for setting the input
        */
        virtual void fetch_input() {}
        /**
         * Optional function which will be executed after each iteration and processing of transitions
         * Intended for generating logging or outputs that are needed in all states like UDP
        */
        virtual void post_iterate() {}
};


#endif


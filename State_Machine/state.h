#ifndef __STATE_H__
#define __STATE_H__

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
enum class State_Code {Startup, Forward, Branch, Backtrack, Gap, Detection, Stop, _Count};

enum class Branch_Direction {Right, Left};

enum class Return_Value {Loop, Timeout, Intersection, Line_Loss, Complete, _Count};

template<typename I, typename O>
class Abstract_State {
    public: 
        explicit Abstract_State(State_Code code) : code(code) {}

        const State_Code code;

        virtual void entry_behavior(I&, O&) {}
        virtual void do_behavior(I&, O&) {}
        virtual void exit_behavior(I&, O&) {}
        virtual Return_Value guard(I&) = 0;
};

#endif
#include <iostream>
#include "tempi/fsm/statemachine.h"

using namespace tempi::fsm;

static const bool VERBOSE = false;

bool check_fsm()
{
    StateMachine machine;
    machine.addState(new State("foo"));
    State *foo = machine.getState("foo");
    if (machine.getCurrentStateName() != "foo")
    {
        std::cout << "State should be set to the first we add when we add it." << std::endl;
        std::cout << "State is " << machine.getCurrentStateName() << std::endl;
        return false;
    }

    machine.addState(new State("bar"));
    State *bar = machine.getState("bar");
    if (machine.getCurrentStateName() != "foo")
    {
        std::cout << "State should remain the same when we add a new one." << std::endl;
        std::cout << "State is " << machine.getCurrentStateName() << std::endl;
        return false;
    }

    machine.setCurrentStateName("foo");
    if (machine.getCurrentStateName() != "foo")
    {
        std::cout << "State should be changed when we call setCurrentStateName." << std::endl;
        std::cout << "State is " << machine.getCurrentStateName() << std::endl;
        return false;
    }

    machine.addRule(new Rule("bang!", "foo", "bar"));
    machine.triggerEvent("bang!");
    if (machine.getCurrentStateName() != "bar")
    {
        std::cout << "State should be changed when we trigger the event." << std::endl;
        std::cout << "State is " << machine.getCurrentStateName() << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (! check_fsm())
        return 1;
    return 0;
}


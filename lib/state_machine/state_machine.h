/*
 * Header file for StateMachine class. Declare class, methods, variables
 *
 * Colton W
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
 * @brief   Create state machines for sensor/button inputs
 *
 * @note
 *
 *
 */
class StateMachine {

    public:

        // Constructors
        StateMachine(int pin);
        StateMachine(int pin, int initial_state);

        // Helper Funcs
        int update_state();
        int update_state_no_delay();
        void print_state();

        // Variables
        unsigned int pin_number;
        unsigned int state;
        unsigned int debounce_period = 10; // 10 ms wait on reads for debounce period
        // uint64_t last_keypress = 0;

};

#endif // STATE_MACHINE_H
/*
 * Definitions for functions declared in state_machine.h
 *
 * Colton W
 */

#include <stdio.h>
#include <Arduino.h>
#include <string>
#include "state_machine.h"

StateMachine::StateMachine(int pin) {
    pin_number = pin;

    pinMode(pin, INPUT);
    state = 3;
}

StateMachine::StateMachine( int pin, int initial_state) {
    pin_number = pin;
    state = initial_state;

    pinMode(pin, INPUT);
    state = initial_state;
}

/**
 * @brief   Debounce included for buttons, sensors, etc.
 */
int StateMachine::update_state() {
    bool read1 = digitalRead(pin_number);
    delay(debounce_period);
    bool read2 = digitalRead(pin_number);

    // Check for debounce, do not update state if not steady reading over debounce period
    if (read1 != read2) {
        return(0);
    }

    // No debounce, update state machine...
    if (state == 1 && read1 == HIGH) {
        state = 2;
    }
    else if (state == 2) {
        state = 3;
    }
    else if (state == 3 && read1 == LOW) {
        state = 4;
    }
    else if (state == 4) {
        state = 1;
    }
    // What happened?... Normally closed button, starts with pin HIGH. Button is pressed, send state
    // 4, a transition state, then finally state 1, button is held pressed, LOW. If button released,
    // sent to state 2, a transition state, then to state 3, button released, HIGH.

    return(0);
}

/**
 * @brief   Only for comparator/solid state switches with built-in hysteresis, no hardware debounce
 */
int StateMachine::update_state_no_delay() {
    bool read1 = digitalRead(pin_number);

    // No debounce, update state machine...
    if (state == 1 && read1 == HIGH) {
        state = 2;
    }
    else if (state == 2) {
        state = 3;
    }
    else if (state == 3 && read1 == LOW) {
        state = 4;
    }
    else if (state == 4) {
        state = 1;
    }

    return(0);
}
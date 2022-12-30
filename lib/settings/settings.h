/*
 * Header file for Settings class. Declare class, methods, variables
 *
 * Colton W
 */

#ifndef SETTINGS_H
#define SETTINGS_H


/**
 * @brief   Settings class that saves user input to effect coil params
 *
 * @note    Settings are edited by incrementing/decrementing the current setting selected
 *          by setting_select. The settings are listed in order in the class declaration.
 *          fire_mode = 1 Manual Fire
 *          fire_mode = 2 Auto Fire
 *
 *
 */
class Settings {

    public:

        // Constructors
        Settings();

        // Helper funcs
        void increment_setting_select();
        void increment_setting();

        void decrement_setting_select();
        void decrement_setting();

        void print_settings();

        // Variables
        unsigned int setting_select;        // Select which setting to change

        unsigned int fire_mode;             // Setting #1
        unsigned int coil_fire_time_ms;     // Setting #2
        unsigned int cross_delay_ms;        // Setting #3
        unsigned int autofire_time_ms;      // Setting #4
        
};

#endif // SETTINGS_H
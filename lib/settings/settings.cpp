/*
 * Definitions for functions declared in settings.h
 *
 * Colton W
 */

#include "settings.h"
#include <Arduino.h>

// Default constructor, our default settings chosen on boot
Settings::Settings() {

        setting_select = 1;

        coil_fire_time_ms = 10;
        cross_delay_ms = 0;
        fire_mode = 1;
        autofire_time_ms = 5000;
}

/**
 * @brief   Increment the setting to print/edit
 */
void Settings::increment_setting_select() {
    setting_select += 1;

    if (setting_select >= 5){ // Only 3 settings to change at the moment.
        setting_select = 1;
    }
}

/**
 * @brief   Decrement the setting to print/edit
 */
void Settings::decrement_setting_select() {
    setting_select -= 1;

    if (setting_select == 0){ // This won't go to 0. Can't go negative.
        setting_select = 1;
    }
}


/**
 * @brief   Increase the current setting
 *
 *
 */
void Settings::increment_setting() {
    switch(setting_select) {
        case 1:
            fire_mode += 1;

            if(fire_mode > 2) {
                fire_mode = 2;
            }

            break;

        case 2:
            coil_fire_time_ms += 1;

            if (coil_fire_time_ms > 150) {
                coil_fire_time_ms = 150;
            }

            break;

        case 3:
            cross_delay_ms += 1;

            if(cross_delay_ms > 16) {
                cross_delay_ms = 16;
            }

            break;

        
        case 4:
            autofire_time_ms += 100;

            break;

        default:

            // Setting select is broken...
            break;
    }
}

/**
 * @brief   Decrease the current setting
 *
 *
 */
void Settings::decrement_setting() {
    switch(setting_select) {
        case 1:
            fire_mode -= 1;

            if(fire_mode < 1) {
                fire_mode = 1;
            }

            break;

        case 2:
            coil_fire_time_ms -= 1;

            if (coil_fire_time_ms < 1) {
                coil_fire_time_ms = 1;
            }

            break;

        case 3:
            cross_delay_ms -= 1;

            if(cross_delay_ms < 1) {
                cross_delay_ms = 1;
            }

            break;

        
        case 4:
            autofire_time_ms -= 100;

            if(autofire_time_ms < 1000) {
                autofire_time_ms = 1000;
            }

            break;

        default:

            // Setting select is broken...
            break;
    }
}

/**
 * @brief   Print current settings to serial monitor
 *
 *
 */
// TODO:
void Settings::print_settings() {
    switch (setting_select)
    {
    case 1:
        if (fire_mode == 1)
        {
            Serial.print("Fire mode: Manual\n");
        }
        else {
            Serial.print("Fire mode: Automatic\n");
        }
        
        break;

    case 2:
        Serial.print("Coil fire time is: ");
        Serial.print(coil_fire_time_ms);
        Serial.print(" ms\n");
        break;

    case 3:
        Serial.print("Cross fire delay is: ");
        Serial.print(cross_delay_ms);
        Serial.print(" ms\n");
        break;

    case 4:
        Serial.print("Auto Fire time is: ");
        Serial.print(autofire_time_ms);
        Serial.print(" ms\n");
        break;
    
    default:
        break;
    }

}
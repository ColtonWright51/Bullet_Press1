#include <Arduino.h>
#include "settings.h"
#include "state_machine.h"

#define led_pin 13

#define coil_pin 38
#define out1 37 // Armature retract
#define out2 36 // Armature extend
#define out3 35 // Cycle part
#define out4 34 // Cycle part
#define out5 35 // Cycle part

#define in1 27 //26 // Toggle switch
#define in2 26 //27 // Button1, yellow
#define in3 28 // Button2, green
#define in4 29 // Button3, green
#define estop_in 30 //30

#define comparator_in 0
#define hall1_in 39
#define hall2_in 40

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(out1, OUTPUT); // Armature retract
  pinMode(out2, OUTPUT); // Armature extend
  pinMode(out3, OUTPUT); // Cycle part
  pinMode(out4, OUTPUT); // Cycle part
  pinMode(out5, OUTPUT); // Cycle part

  pinMode(coil_pin, OUTPUT);

  Serial.begin(9600);

}

void loop() {


  delay(5000);

  Serial.print("Hello World!\r\n");
  digitalWrite(led_pin, HIGH);

  // Initialize settings object
  Settings set;

  // Button Inputs
  StateMachine sm1(in1); // Toggle switch
  StateMachine sm2(in2); // Button1, yellow
  StateMachine sm3(in3); // Button2, green
  StateMachine sm4(in4); // Button3, green
  StateMachine estop(estop_in);

  // Sensors
  StateMachine comparator(comparator_in);
  StateMachine hall1(hall1_in); // Retracted sensor
  StateMachine hall2(hall2_in); // Extended sensor

  while (true) {

    unsigned long current_time = millis();  // WILL OVERFLOW IN 50 DAYS
    
    sm1.update_state();
    estop.update_state();
    
    if (estop.state == 1) {
      Serial.print("Emergency Stop 1!\n");
      while(true) {
        estop.update_state();
        if (estop.state == 3) {
          Serial.print("Leaving Estop...\n");
          break;
        }
      }
    }


    // Settings loop...
    bool is_entering_settings = 1;
    while (sm1.state == 1) {

      if (is_entering_settings == 1) {
        Serial.print("Entering settings...\n");
        is_entering_settings = 0;
      }

      // current_time = millis(); // For data saving later?
      sm1.update_state();
      sm2.update_state();
      sm3.update_state();
      sm4.update_state();
      estop.update_state();

      static bool is_recent_change = 1;
      if (sm2.state == 4) {
        set.increment_setting_select();
        is_recent_change = 1;
      }
      if (sm3.state == 4) {
        set.increment_setting();
        is_recent_change = 1;
      }
      if (sm4.state == 4) {
        set.decrement_setting();
        is_recent_change = 1;
      }
      if (is_recent_change == 1) {
        set.print_settings();
        is_recent_change = 0;
      }
    }



    // Manual fire loop...
    bool is_entering_manualfire = 1;
    while (sm1.state == 3 && set.fire_mode == 1){

      if (is_entering_manualfire == 1) {
        Serial.print("Entering Manual Fire...\n");
        is_entering_manualfire = 0;
      }

      estop.update_state();
      if (estop.state == 1 || estop.state == 4) {
        Serial.print("Emergency Stop 2!\n");
        while (true) {
          estop.update_state();
          if (estop.state == 3) {
            Serial.print("Leaving Estop...\n");
            break;
          }
        }
      }

      current_time = millis();
      static unsigned long last_fire = millis();
      static bool is_fired = 0;

      sm1.update_state(); // To leave loop when switch is flipped
      sm2.update_state(); // For fire signal... Danger!

      if (sm2.state == 1 && current_time - last_fire > 2000 && sm1.state == 3) {
        is_fired = 0;
        while (is_fired == 0) {

          comparator.update_state_no_delay(); // No debounce on comparator, & require fast updates

          if (comparator.state == 2) { // Trigger on rising edge of comparator input
            delay(set.cross_delay_ms);
            digitalWrite(coil_pin, HIGH);
            delay(set.coil_fire_time_ms);
            digitalWrite(coil_pin, LOW);
            last_fire = millis(); // Update timer so can't fire again soon
            is_fired = 1; // Break out of fire loop
            Serial.print("FIRE!\n");
          }
        }
      }
    }



    // Auto fire loop...
    bool is_entering_autofire = 1;
    while (sm1.state == 3 && set.fire_mode == 2){

      current_time = millis();
      static unsigned long last_fire = millis();
      static bool is_fired = 0;
      static bool can_fire = 1; // Do not fire the machine if armature is stuck for 10s and then retracted

      if (is_entering_autofire == 1) {
        Serial.print("Entering Auto Fire...");
        is_entering_autofire = 0;
      }

      estop.update_state();
      if (estop.state == 1 || estop.state == 4) {
        Serial.print("Emergency Stop 3!\n");
        while (true) {
          estop.update_state();
          if (estop.state == 3) {
            Serial.print("Leaving Estop...\n");
            break;
          }
        }
      }

      hall1.update_state(); // TODO: Check inputs of hall on actual machine!
      if (hall1.state == 1 ) {
        Serial.print("Armature is stuck!\n");
        Serial.print("Press button3 to retract armature");
        can_fire = 0; // DO NOT FIRE WHEN RETRACTED!
        while (true) {
          hall1.update_state();
          sm3.update_state();
          if (sm3.state == 1) {
            digitalWrite(out1, HIGH);
            delay(500); // TODO: How long should the pneumatics actuate the armature?
            digitalWrite(out1, LOW);
          }

          if (hall1.state == 3) {
            break;
          }


        }
      }

      if (can_fire == 0) {
        Serial.print("Cannot fire. Press button2 to reset autofire");
        while (true) {
          sm3.update_state();
          if (sm3.state == 1) {
            break;
            last_fire = millis(); // update last_fire so it does not fire until time elapses
          }
        }
      }

      sm1.update_state(); // To leave loop when switch is flipped
      current_time = millis();

      if (current_time - last_fire >= set.autofire_time_ms && can_fire == 1 && sm1.state == 3) { // Begin fire sequence...

      }










    }

  }
}
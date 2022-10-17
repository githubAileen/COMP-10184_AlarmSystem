// COMP-10184 – Mohawk College 
// Alarm System 
//  
// This program is an alarm system where in it detects motion and button inputs from
// a sensor and does a specific action per state.
// 
// @author  Aileen Velasco 
// @id      000805966 
// 
// I created this work and I have not shared it with anyone else. 
// 
 
#include <Arduino.h>
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6 

// define button delay
#define BUTTON_DELAY 200 

// define all LED states.
#define LED_ON   0
#define LED_OFF  1 

// define all possible alarm states. 
#define ALARM_DEFAULT   0
#define ALARM_DISABLED  1 
#define ALARM_ENABLED   2 
#define ALARM_COUNTDOWN 3 
#define ALARM_ACTIVE    4 

// global variables
int alarmState = 0;
int buttonInput = 0;
bool buttonState = false;
bool sensorInput = false;

// *********************************************************** 
// Check if any sensor or button input has been read and change their state accordingly
// 
void getInputs() { 
  // read PIR sensor input
  sensorInput = digitalRead(PIN_PIR);
  // read button input
  buttonInput = digitalRead(PIN_BUTTON);

  //Check if sensor has been activated and if alarm state is not disabled
  if(sensorInput && alarmState != ALARM_DISABLED){
    alarmState = ALARM_COUNTDOWN;
  }

  //Check if button is pressed
  if(buttonInput == 0){
    delay(BUTTON_DELAY);

    // check if button was pressed during countdown
    if(alarmState == ALARM_COUNTDOWN){
      buttonState = true;
    }
    // check if button has been pressed while alarm was disabled
    else if (alarmState == ALARM_DISABLED)
    {
      buttonState = false;
      alarmState = ALARM_ENABLED;
    }
  }
}
  
// *********************************************************** 
// This function checks the alarm state and does an action per state
// 
void checkAlarmState() { 
  
  // check button state
  if(buttonState == false){

    switch (alarmState) {
      case ALARM_DISABLED:
        disable_alarm:
        // display when alarm is disabled
        Serial.println("\nButton Pressed - Alarm Disabled");
        // LED OFF
        digitalWrite(LED_BUILTIN, LED_OFF);
        break;
      case ALARM_ENABLED:
        // display when alarm is enabled
        Serial.println("\nButton Pressed - Alarm Enabled");
        alarmState = ALARM_DEFAULT;
        //sensorInput = 0;
        break;
      case ALARM_COUNTDOWN:
        // display when alarm is on countdown
        Serial.println("\nMotion Detected - Alarm will activate in 10s!\n");

        // when alarm is activated, countdown begins
        for(int x = 0; x < 10; x++){
            // let light blink 4 times in one second
            // (1000ms / 4 = 250ms) / 2 = 125ms to blink 4 times
            for(int y = 0; y < 4; y++){
                  // check for button press
                  getInputs();
                  
                  // if button has been pressed disable alarm
                  if(buttonState == true) {
                    alarmState = ALARM_DISABLED;
                    goto disable_alarm;
                  }

                  // LED ON
                  digitalWrite(LED_BUILTIN, LED_ON);
                  delay(125);

                  // LED OFF
                  digitalWrite(LED_BUILTIN, LED_OFF);
                  delay(125);
              }

              //Print seconds
              Serial.print(String(x+1));
        }

        //set alarm state to activate after 10 seconds
        alarmState = ALARM_ACTIVE;
        break;
      case ALARM_ACTIVE:
        // display when alarm is activated
        Serial.println("\nAlarm Activated!");
        // toggle button state so that user won't be able to do anymore actions
        buttonState = true;
        // LED ON
        digitalWrite(LED_BUILTIN, LED_ON);
        break;
      default:
        // Do Nothing
        break;
    }
  }
} 

// *********************************************************** 
void setup() { 
  // configure the USB serial monitor
  Serial.begin(115200);
 
  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);
 
  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);
 
  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Print Header
  Serial.println("\n===========================================");
  Serial.println("\nCOMP-10184 – Alarm System");
  Serial.println("Name: Aileen Velasco");
  Serial.println("Student ID: 000805966");
  Serial.println("\n===========================================");

}

// *********************************************************** 
void loop(){ 
  // get button and motion detector inputs 
  getInputs();

  // should we arm/activate/enable/disable the alarm?
  checkAlarmState();
} 
 

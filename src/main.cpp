#include <Arduino.h>
#include "Setting.h"
#include "Mymqtt.h"
#include <Button.h>        //https://github.com/JChristensen/Button
#include <Bounce2.h>

#define LEVEL_PIN D4
//#define LED_PIN 13

#define BUTTON_PIN D3       //Connect a tactile button switch (or something similar)
                           //from Arduino pin 2 to ground.
#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
                           //switch is closed, this is negative logic, i.e. a high state
                           //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define LED_PIN D7         //The standard Arduino "Pin 13" LEDwemos D7

Button myBtn(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
boolean ledState;          //A variable that keeps the current LED status

Mymqtt mqtt= Mymqtt();
// Instantiate a Bounce object
Bounce debouncer = Bounce();

void setup() {
    // put your setup code here, to run once:

    mqtt.connect();
    pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output
    // Setup the button with an internal pull-up :
    pinMode(BUTTON_PIN,INPUT_PULLUP);
    pinMode(LEVEL_PIN,INPUT_PULLUP);

    // After setting up the button, setup the Bounce instance :
    debouncer.attach(BUTTON_PIN);
    debouncer.interval(5); // interval in ms

    //Setup the LED :
  //  pinMode(LED_PIN,OUTPUT);

}

void loop() {
    // put your main code here, to run repeatedly:
  if(!mqtt.loop()){

    mqtt.reconnect();
 }
 myBtn.read();                    //Read the button

 if (myBtn.wasReleased()) {       //If the button was released, change the LED state
     ledState = !ledState;
     digitalWrite(LED_PIN, ledState);
     mqtt.botonpress(ledState);
 }
 // Update the Bounce instance :
 debouncer.update();

 // Get the updated value :
 int value = debouncer.read();

 // Turn on or off the LED as determined by the state :
 if ( value == LOW ) {
   digitalWrite(LED_PIN, HIGH );
   mqtt.highlevel();
 }
 else {
   digitalWrite(LED_PIN, LOW );
   mqtt.lowlevel();
 }
  //ESP.deepSleep(20e6); //20e6 20 second

  delay(500);// 5seconds
}

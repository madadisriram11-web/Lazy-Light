#include <Arduino.h>
#include <Servo.h>
#include <IRremote.h>

const int SERVO_PIN = 9; // Servo GPIO pin
const int IR_RECEIVE_PIN = 11; // IR sensor GPIO pin
const int BUTTON_PIN = 4;

Servo switchServo; // sets up the servo commands
bool lastButtonState = HIGH;
bool lightState = false; // sets up wheter the light is on or off

const int ANGLE_OFF = 45; // this is the angle when thelight is off
const int ANGLE_ON = 135; // this is the angle when the light is on

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //starting up the IR reciever
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  //setting up the servo
  switchServo.attach(SERVO_PIN);
  switchServo.write(ANGLE_OFF);

  Serial.println("System Ready. Waiting for IR signal . . .");
}

void updateLightState() {
  if (lightState) {
    Serial.println("Turning Light ON");
    switchServo.write(ANGLE_ON);
  } else {
    Serial.println("Turning Light OFF");
    switchServo.write(ANGLE_OFF);
  }
  delay(300);
}

void loop() {
  if (IrReceiver.decode()) {
    // just setting up to send to the computer to tell if the program is working or not
    Serial.print("IR Command Code: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX); // ir is in hexadecimal

    // checking the light state and moving the servo accordingly
    if (IrReceiver.decodedIRData.command == 0x1F) { // 0x1F is just a placeholder for the 
      // real hexadecimal # that my remote will use
      lightState = !lightState;
      updateLightState();
    }
    
    IrReceiver.resume(); // letting teh button pushing action not just be 1 time

  }

  bool currentButtonState = digitalRead(BUTTON_PIN);
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    lightState = !lightState;
    updateLightState();
    delay(300);
  }
  lastButtonState = currentButtonState;
}
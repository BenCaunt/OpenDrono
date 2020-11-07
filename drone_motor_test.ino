#include <Servo.h>
Servo ESC;     // create servo object to control the ESC
int pwmSpeed;  // value from the analog pin
int convertedSpeed; 
unsigned long start_of_motor_arm_first = 0;
unsigned long start_of_motor_arm_second = 0;
unsigned long time_for_motor_arm = 4000;
long state = 0;
void setup() {
  Serial.begin(9600);
  // Attach the ESC on pin 9
  ESC.attach(A0,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 


  delay(1000);
  start_of_motor_arm_first = millis();
  state = 1;


}
void loop() {
    if (start_of_motor_arm_first + time_for_motor_arm > millis()) {
      state = 2;
      pwmSpeed = 0; // reads the value of the potentiometer (value between 0 and 1023)
    } else if (start_of_motor_arm_first + (time_for_motor_arm * 2) > millis()) {
      state = 3;
      pwmSpeed = 1023; // reads the value of the potentiometer (value between 0 and 1023)
    } else {
      state = 4;
      pwmSpeed -= 1;
    }
    convertedSpeed = map(pwmSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESC.write(convertedSpeed);    // Send the signal to the ESC
    Serial.println(state);
    delay(10);
}

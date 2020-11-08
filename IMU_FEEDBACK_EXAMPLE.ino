#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)
// see: https://photos.app.goo.gl/ynoUNaPzgd3NF1QU9
// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
Servo ESC;     // create servo object to control the ESC
int pwmSpeed;  // value from the analog pin
int convertedSpeed; 
int pwmSpeed_base; 
unsigned long start_of_motor_arm_first = 0;
unsigned long start_of_motor_arm_second = 0;
unsigned long time_for_motor_arm = 4000;
long state = 0;
void setup() {
  Serial.begin(9600);
  // Attach the ESC on pin 9
  ESC.attach(A0,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
  // get current time for motor calibration stuff 
  start_of_motor_arm_first = millis();
  state = 1;


}
void loop() {
    if (start_of_motor_arm_first + time_for_motor_arm > millis()) {
      state = 2;
      pwmSpeed = 0; // reads the value of the potentiometer (value between 0 and 1023)
    } else if (start_of_motor_arm_first + (time_for_motor_arm * 2) > millis()) {
      state = 3;
      pwmSpeed = 522; // reads the value of the potentiometer (value between 0 and 1023)
    } else {
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

      state = 4;
      pwmSpeed = 522 - euler.x();
    }
    convertedSpeed = map(pwmSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESC.write(convertedSpeed);    // Send the signal to the ESC
    Serial.println(state);

    delay(10);
}

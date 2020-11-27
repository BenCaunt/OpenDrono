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
Servo FrontLeftESC;     // create servo object to control the ESC
Servo FrontRightESC; 
Servo BackLeftESC;
Servo BackRightESC;
int pwmSpeed;  // value from the analog pin
int convertedSpeed; 
int pwmSpeed_base; 
unsigned long start_of_motor_arm_first = 0;
unsigned long start_of_motor_arm_second = 0;
unsigned long time_for_motor_arm = 4000;
long state = 0;


double yawPower = 0;
double pitchPower = 0;
double rollPower = 0;
double heightPower = 0; 

double frontRightSpeed = 0;
double frontLeftSpeed = 0;
double backRightSpeed = 0;
double backLeftSpeed = 0;

double x_angle_calibration = 0;
double y_angle_calibration = 0;
double z_angle_calibration = 0;






double pitchAngleWrap(double angle) 
{
  return angle;
}

double yawAngleWrap(double angle)
{
  if (angle < 110) {
    angle = 200 - angle;
  }
  return angle;
}

double rollAngleWrap(double angle) 
{

  if (angle < 0) {
    angle += 360;
  }
  return angle;
}

void heightController(double setpoint, double measurement) {

  double error = setpoint - measurement;
  heightPower = 0;
  
}



void yawController(double setpoint, double measurement) {
  
  double error = setpoint - yawAngleWrap(measurement);
  yawPower = error;   
}

void pitchController(double setpoint, double measurement) {
  
  double error = setpoint - measurement;
  pitchPower = error;
  
}

void rollController(double setpoint, double measurement) {

  double error = (setpoint - rollAngleWrap(measurement));
  rollPower = error;
  
}


void setup() {
  Serial.begin(9600);
  // Attach the ESC on pin 9
  FrontLeftESC.attach(A0,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  FrontRightESC.attach(A1,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  BackLeftESC.attach(A2,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  BackRightESC.attach(A3,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 

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
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    if (start_of_motor_arm_first + time_for_motor_arm > millis()) {
      x_angle_calibration = euler.x();
      y_angle_calibration = euler.y();
      z_angle_calibration = euler.z();
      state = 2;
      pwmSpeed = 0; // reads the value of the potentiometer (value between 0 and 1023)
    } else if (start_of_motor_arm_first + (time_for_motor_arm * 2) > millis()) {
      state = 3;
      pwmSpeed = 522; // reads the value of the potentiometer (value between 0 and 1023)
    } else {
      rollController(z_angle_calibration,euler.z());
      pitchController(y_angle_calibration,euler.y());
      yawController(x_angle_calibration, euler.x());
      state = 4;
      pwmSpeed = 522 - euler.x();
    }
    frontRightSpeed = heightPower - rollPower - pitchPower - yawPower;
    frontLeftSpeed = heightPower + rollPower - pitchPower + yawPower;
    backRightSpeed = heightPower - rollPower + pitchPower + yawPower;
    backLeftSpeed = heightPower + rollPower + pitchPower - yawPower; 

    
    FrontLeftESC.write(frontLeftSpeed);    // Send the signal to the ESC
    FrontRightESC.write(frontRightSpeed);
    BackLeftESC.write(backLeftSpeed);
    BackRightESC.write(backRightSpeed);
    Serial.println(state);
    Serial.print("heightPower is: ");
    Serial.println(heightPower);
    Serial.print("rollPower is: ");
    Serial.println(rollPower);
    Serial.print("pitchPower is: ");
    Serial.println(pitchPower);
    Serial.print("yawPower is: ");
    Serial.println(yawPower);
    Serial.print("x: ");
    Serial.print(euler.x());
    Serial.print(" y: ");
    Serial.print(euler.y());
    Serial.print(" z: ");
    Serial.print(euler.z());
    delay(10);
}

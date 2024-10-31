#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4IMU imu;
Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Motors motors;

// vars to count the accumulated counts of the encoders
int movementCommand = 0;   // used to select the command
int movementParameter = 0;   // used to select distance or angle

int const countJump = 100;  // number of counts before jumping to next command in stage 0
int const maxDist = 100;   // max distance the robot should travel
int const maxAngle = 359; // max angle the robot can turn
int speed = 100; // max speed of the robot
float wheelCirc = 13.0;
// control the flow of the program. 0 wait for command
//                                  1 wait for speed and time
//                                  2 running the command.
int stage = 0;      

// control selection 1              0 Forward
//                                  1 Backwards
//                                  2 turn
int chosenCommand = 0;

const int sizeA = 5;
const int sizeB = 2;
int commandArray[sizeA][sizeB]; // Array holds five entries of [Command, parameter]

// variables for gyro
uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;

void setup() {
  Serial.begin(9600);
  turnSensorSetup();
  delay(100);
}

void loop() {

}

void programMovements(){

}

void executeProgram(){

}

/*
Button A: repeat program
Button B: reprogram
*/
void userInput(){

}

void selectMovement(){

}

void selectParameter(){

}

/*
Movement functions
*/

void stop(){
  motors.setSpeeds(0,0);
}

void forward(){
  motors.setSpeeds(speed, speed);
}

void backward(){
  motors.setSpeeds(-speed, -speed);
}

void turnByAngle(){

}

void executeMovement(){

}

/*
LCD functions
*/
void LCDSelectMovement(){

}

void LCDSelectParameter(){

}


// Easy fucntion to print in the LCD
void printLCD(String s0, String s1){
  lcd.clear();
  lcd.print(s0);
  lcd.gotoXY(0,1);
  lcd.print(s1);
}

/*
Encoder functions
*/

float getDistance(){
  int countsL = encoders.getCountsLeft();
  int countsR = encoders.getCountsRight();

  float distanceL = countsL/900.0 * wheelCirc;
  float distanceR = countsR/900.0 * wheelCirc;

  return (distanceL + distanceR)/2;
}

void resetEncoders(){
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

void readEncodersMovement(){

}

void readEncodersParameter(){

}

/*
Sound functions
*/

void bip(){

}

void countdown(){

}

/* 
Gyro setup and convenience functions '
*/
void turnSensorSetup()
{
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  lcd.clear();
  lcd.print(F("Gyro cal"));

  // Turn on the yellow LED in case the LCD is not available.
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++)
  {
    // Wait for new data to be available, then read it.
    while(!imu.gyroDataReady()) {}
    imu.readGyro();

    // Add the Z axis reading to the total.
    total += imu.g.z;
  }
  ledYellow(0);
  gyroOffset = total / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  lcd.clear();
  turnSensorReset();
}

// This should be called to set the starting point for measuring
// a turn.  After calling this, turnAngle will be 0.
void turnSensorReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
void turnSensorUpdate()
{
  // Read the measurements from the gyro.
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  int32_t d = (int32_t)turnRate * dt;

  // The units of d are gyro digits times microseconds.  We need
  // to convert those to the units of turnAngle, where 2^29 units
  // represents 45 degrees.  The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  //
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  turnAngle += (int64_t)d * 14680064 / 17578125;
}

uint32_t getTurnAngleInDegrees(){
  turnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((uint32_t)turnAngle >> 16) * 360) >> 16;
}


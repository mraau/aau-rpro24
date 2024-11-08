#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;

struct RoverControl{
  char command;
  int speed;
  int duration;
};

const int arraySize =5;
char motorCommand[] ={'f','r','f','l','b'};
int spDur[][2] = {{400,600},{300,300},{400,200},{300,400},{300,800}};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void stopRobot(){
  motors.setSpeeds(0, 0);
}

void moveForward(int mspeed){
  motors.setSpeeds(mspeed, mspeed);
}

void moveBackward(int mspeed){
  moveForward(-mspeed); 
}

int pauseLength(){
  return random(50, 2000);
}

void loop() {

  RoverControl rc[arraySize];

  for (int i=0; i< arraySize; i++){
    rc[i].command= 'f';
    rc[i].speed = 100;
    rc[i].duration = 200;
  }


  for(int i=0;i<arraySize;i++){
    switch(rc[i].command){
      case 'f': moveForward(rc[i].speed); break;
      case 'b': moveBackward(rc[i].speed);break;
      case 'r': motors.setSpeeds(-rc[i].speed, rc[i].speed); break;
      case 'l': motors.setSpeeds(rc[i].speed, -rc[i].speed);break;
      default: stopRobot();
    }
    delay(rc[i].duration);
    stopRobot();
  }

  int pl = pauseLength();
  delay(pl);
}

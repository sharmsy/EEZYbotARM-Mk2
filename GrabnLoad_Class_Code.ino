/*
05.06.2025

Coding & Robotics Class
Aldersgate Christian Academy
Mr. Harms

Starter code for operating the EEZYbotARM Mk 2 with a Playstation 2 Controller
and automatically loading a delivery vehicle
*/
#include <PS2X_lib.h>   
#include <Adafruit_PWMServoDriver.h>
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
PS2X ps2x; 
Adafruit_PWMServoDriver servoboard = Adafruit_PWMServoDriver();
int closed = 218;
int open = 75;
int d = 15; //micro delay when controlling manually
int ad = 5; //micro delay for slowing down the movements in the functions used by the scripted movements
int error = 0; 
byte type = 0;
byte vibrate = 0;

#define servoMINbase 80 //how far the base turns left
#define servoMAXbase 675 ////how far the base turns right
int servoMIDbase = (servoMAXbase-servoMINbase)/2+servoMINbase;
int basepos = 377;

#define servoMINarm 300 //250 //how far back it goes
#define servoMAXarm 600 //500 //how far out it reaches out
int servoMIDarm= (servoMAXarm-servoMINarm)/2+servoMINarm;
int armpos = 305;

#define servoMINboom 100 //190 //how low the boom arm reaches
#define servoMAXboom 410 //400 //how high the boom goes
int servoMIDboom = (servoMAXboom-servoMINboom)/2+servoMINboom;
int boompos = 405;

#define servoMINclaw 80 //how far the claw opens
#define servoMAXclaw 540 //how far the claw closes
int servoMIDclaw = (servoMAXbase-servoMINbase)/2+servoMINbase;
int clawpos = servoMINclaw;

//adjust the following for your specific robot arm
int closed = 218; //the value for a closed claw
int open = 75; //the value for an open claw

//the following are the coordinates to pick up the block
int grabBase = 600; 
int grabArm = 513;
int grabBoom = 138;

//the following are the coordinates for releasing the block on the vehicle
int releaseBase = 316;
int releaseArm = 454;
int releaseBoom = 324;


  void grabNLoad()
  { 
    moveClaw(open);
    moveBase(grabBase);
    
    moveArm(440);
    moveBoom(191);
    moveArm(457);
    moveBoom(grabBoom);
    moveArm(grabArm);
    moveClaw(closed);
    moveArm(450);
    moveBoom(220);
    moveArm(405);
    moveBoom(servoMAXboom);
    moveBase(releaseBase);
    moveArm(releaseArm);
    moveBoom(releaseBoom);
    moveClaw(open);
    moveBoom(servoMAXboom);
    moveArm(304);
    moveBase(servoMIDbase);

  }
  
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets pin 3 as an OUTPUT
  pinMode(echoPin, INPUT); // Sets pin 2 as an INPUT
  servoboard.begin();
  servoboard.setPWMFreq(60);
  //set initial servo positions
  servoboard.setPWM(0, 0, basepos);
  servoboard.setPWM(1, 0, armpos);
  servoboard.setPWM(2, 0, boompos);
  servoboard.setPWM(3, 0, 110);
  error = ps2x.config_gamepad(4,6,5,7, false, false); 
   if(error == 0){
   Serial.println("Found Controller, configured successful");
 }
}

void loop() {
  // Serial.print("About to get the distance");
  int distance = getDistance();
  // Serial.print("The distance is: ");
  // Serial.println(distance);

  if(getDistance() < 1)
  {
    delay(2000);
    if(getDistance() < 25)
    { 
      grabNLoad();
    }
  }

  ps2x.read_gamepad(false, vibrate);
// Serial.println("before base right");
  if(ps2x.Analog(PSS_LX) > 140)
  {
    Serial.println("base right");
    baseRight();
  }
// Serial.println("before base left");
  if(ps2x.Analog(PSS_LX) < 120)
  {
    Serial.println("base left");
    baseLeft();
  }
// Serial.println("before boom up");
 if(ps2x.Analog(PSS_LY) > 140)
  {
    Serial.println("boom up");
    boomUp();
  }
    // Serial.println("before boom down");
  if(ps2x.Analog(PSS_LY) < 120)
  {
    Serial.println("boom down");
    boomDown();
  }
    // Serial.println("before arm forward");
  if(ps2x.Analog(PSS_RY) > 140)
  {
    Serial.println("arm forward");
    armForward();
  }
    // Serial.println("before arm back");
  if(ps2x.Analog(PSS_RY) < 120)
  {
    Serial.println("arm back");
    armBackward();
  }
    // Serial.println("before up pressed");  
  if(ps2x.Button(PSB_PAD_UP)) {
    Serial.println("up pressed");
    openClaw();
  }
    // Serial.println("before down pressed");
  if(ps2x.Button(PSB_PAD_DOWN)) {
    Serial.println("down pressed");
    closeClaw();
  }
  if(ps2x.Button(PSB_RED)){
   grabNLoad();
  }

  if(ps2x.Button(PSB_BLUE)){
    moveClaw(102);
    moveArm(350);
    moveBase(550);
    moveBoom(240);
    moveArm(470);
    moveClaw(400);
    moveBoom(370);
    moveBase(90);
    moveArm(595);
    moveClaw(177);
  }
  if(ps2x.Button(PSB_R2)){
  
  }
  if(ps2x.Button(PSB_R1)){
    
  }

  if(ps2x.NewButtonState() && ps2x.Button(PSB_SELECT))
  {
    Serial.println("You pressed the Select button. Here are your current servo positions:");
    Serial.print("Base position: ");
    Serial.println(basepos);
    Serial.print("Arm position: ");
    Serial.println(armpos);
    Serial.print("Boom position: ");
    Serial.println(boompos);
    Serial.print("Claw position: ");
    Serial.println(clawpos);
  }
  // Serial.println("End of loop");
} //end of void loop

int getDistance()
{
  int dist;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  dist = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
    return dist;
}

void moveBase(int b)
{
  if(basepos > b)
  {
    for(basepos; basepos >= b; basepos=basepos-1)
    {
      servoboard.setPWM(0, 0, basepos); //moves the servo on connection 0
      Serial.println(basepos);
      delay(ad);
    }
  }

  if(basepos < b)
  {
    for(basepos; basepos <= b; basepos=basepos+1)
    {
      servoboard.setPWM(0, 0, basepos); //moves the servo on connection 0
      Serial.println(basepos);
      delay(ad);
    }
  }
} //end of moveBase() function

void moveArm(int a)
{
  if(armpos > a)
  {
    for(armpos; armpos >= a; armpos=armpos-1)
    {
      servoboard.setPWM(1, 0, armpos); //moves the servo on connection 1
      Serial.println(armpos);
      delay(ad);
    }
  }

  if(armpos < a)
  {
    for(armpos; armpos <= a; armpos=armpos+1)
    {
      servoboard.setPWM(1, 0, armpos); //moves the servo on connection 1
      Serial.println(armpos);
      delay(ad);
    }
  }
} // end of moveArm() function

void moveBoom(int b)
{
  if(boompos > b)
  {
    for(boompos; boompos >= b; boompos=boompos-1)
    {
      servoboard.setPWM(2, 0, boompos); //moves the servo on connection 2
      Serial.println(boompos);
      delay(ad);
    }
  }

  if(boompos < b)
  {
    for(boompos; boompos <= b; boompos=boompos+1)
    {
      servoboard.setPWM(2, 0, boompos); //moves the servo on connection 2
      Serial.println(boompos);
      delay(ad);
    }
  }
} //end of moveBoom() function

void moveClaw(int c)
{
  if(clawpos > c)
  {
    for(clawpos; clawpos >= c; clawpos=clawpos-1)
    {
      servoboard.setPWM(3, 0, clawpos); //moves the servo on connection 3
      Serial.println(clawpos);
      delay(ad);
    }
  }

  if(clawpos < c)
  {
    for(clawpos; clawpos <= c; clawpos=clawpos+1)
    {
      servoboard.setPWM(3, 0, clawpos); //moves the servo on connection 3
      Serial.println(clawpos);
      delay(ad);
    }
  }
} //end of moveClaw() function

void openClaw()  //used for manual control
{
  if (servoMAXclaw > (clawpos + 5))
  { clawpos = clawpos +5;
    servoboard.setPWM(3, 0, clawpos);
    Serial.print("Claw position: ");
    Serial.println(clawpos);
    delay(d);
  }
}

void closeClaw() //used for manual control
{
  if (servoMINclaw < (clawpos - 5))
  { clawpos = clawpos -5;
    servoboard.setPWM(3, 0, clawpos);
    Serial.print("Claw position: ");
    Serial.println(clawpos);
    delay(d);
  }
}

void baseRight() //used for manual control
{
  if (servoMAXbase > (basepos + 5))
  { basepos = basepos +5;
    servoboard.setPWM(0, 0, basepos);
    Serial.print("Base position: ");
    Serial.println(basepos);
    delay(d);
  }
}

void baseLeft() //used for manual control
{
  if (servoMINbase < (basepos - 5))
  { basepos = basepos -5;
    servoboard.setPWM(0, 0, basepos);
    Serial.print("Base position: ");
    Serial.println(basepos);
    delay(d);
  }
}

void boomDown() //used for manual control
{
  if (servoMAXboom > (boompos + 5))
  { boompos = boompos +5;
    servoboard.setPWM(2, 0, boompos);
    Serial.print("Boom position: ");
    Serial.println(boompos);
    delay(d);
  }
}

void boomUp() //used for manual control
{
  if (servoMINboom < (boompos - 5))
  { boompos = boompos -5;
    servoboard.setPWM(2, 0, boompos);
    Serial.print("Boom position: ");
    Serial.println(boompos);
    delay(d);
  }
}

void armBackward() //used for manual control
{
  if (servoMAXarm > (armpos + 5))
  { armpos = armpos +5;
    servoboard.setPWM(1, 0, armpos);
    Serial.print("Arm position: ");
    Serial.println(armpos);
    delay(d);
  }
}

void armForward() //used for manual control
{
  if (servoMINarm < (armpos - 5))
  { armpos = armpos -5;
    servoboard.setPWM(1, 0, armpos);
    Serial.print("Arm position: ");
    Serial.println(armpos);
    delay(d);
  }
}

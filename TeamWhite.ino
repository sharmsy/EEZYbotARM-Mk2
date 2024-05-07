/*
04.30.2024

Coding & Robotics Class
Aldersgate Christian Academy
Mr. Harms

Starter code for operating the EEZYbotARM Mk 2 with a Playstation 2 Controller
The code below is written for a wired remote. To use a wireless remote, use Ctrl-F to find all values of 123 and replace them with 128. There should be six places where you'll do this

*/

#include <PS2X_lib.h>   

#include <Adafruit_PWMServoDriver.h>
PS2X ps2x; 
Adafruit_PWMServoDriver servoboard = Adafruit_PWMServoDriver();

int d = 10;
int error = 0; 
byte type = 0;
byte vibrate = 0;

#define servoMIN 80
#define servoMID 350
#define servoMAX 675

#define servoMINbase 80 //130how far the base turns left
#define servoMAXbase 650 //670//how far the base turns right
int servoMIDbase = (servoMAXbase-servoMINbase)/2+servoMINbase;
int basepos = servoMIDbase;

#define servoMINarm 280 //250 //how far back it goes
#define servoMAXarm 550 //500 //how far out it reaches out
int servoMIDarm= (servoMAXarm-servoMINarm)/2+servoMINarm;
int armpos = servoMIDarm;

#define servoMINboom 220 //190 //how low the boom arm reaches
#define servoMAXboom 500 //400 //how high the boom goes
int servoMIDboom = (servoMAXboom-servoMINboom)/2+servoMINboom;
int boompos = servoMIDboom;

#define servoMINclaw 115 //how far the claw opens
#define servoMAXclaw 540 //how far the claw closes
int servoMIDclaw = (servoMAXbase-servoMINbase)/2+servoMINbase;
int clawpos = servoMIDclaw;

void setup() {
  Serial.begin(9600);
  servoboard.begin();
  servoboard.setPWMFreq(60);
  //set initial servo positions
  servoboard.setPWM(0, 0, basepos);
  servoboard.setPWM(1, 0, armpos);
  servoboard.setPWM(2, 0, boompos);
  servoboard.setPWM(3, 0, clawpos);


  error = ps2x.config_gamepad(9,11,10,12, false, false); 
   if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
}

void loop() {
  ps2x.read_gamepad(false, vibrate);

  if(ps2x.Analog(PSS_LX) > 123)
  {
    baseRight();
  }

  if(ps2x.Analog(PSS_LX) < 123)
  {
    baseLeft();
  }

 if(ps2x.Analog(PSS_LY) > 123)
  {
    boomUp();
  }

  if(ps2x.Analog(PSS_LY) < 123)
  {
    boomDown();
  }
 
  if(ps2x.Analog(PSS_RY) > 139)
  {
    armForward();
  }
  if(ps2x.Analog(PSS_RY) < 123)
  {
    armBackward();
  }
  

  if(ps2x.Button(PSB_PAD_UP)) {
    openClaw();
  }

  if(ps2x.Button(PSB_PAD_DOWN)) {
    closeClaw();
  }
}

void openClaw()
{
  if (servoMAXclaw > (clawpos + 5))
  { clawpos = clawpos +5;
    servoboard.setPWM(3, 0, clawpos);
    Serial.print("Claw position: ");
    Serial.println(clawpos);
    delay(d);
  }
}

void closeClaw()
{
  if (servoMINclaw < (clawpos - 5))
  { clawpos = clawpos -5;
    servoboard.setPWM(3, 0, clawpos);
    Serial.print("Claw position: ");
    Serial.println(clawpos);
    delay(d);
  }
}

void baseRight()
{
  if (servoMAXbase > (basepos + 5))
  { basepos = basepos +5;
    servoboard.setPWM(0, 0, basepos);
    Serial.print("Base position: ");
    Serial.println(basepos);
    delay(d);
  }
}

void baseLeft()
{
  if (servoMINbase < (basepos - 5))
  { basepos = basepos -5;
    servoboard.setPWM(0, 0, basepos);
    Serial.print("Base position: ");
    Serial.println(basepos);
    delay(d);
  }
}

void boomDown()
{
  if (servoMAXboom > (boompos + 5))
  { boompos = boompos +5;
    servoboard.setPWM(2, 0, boompos);
    Serial.print("Boom position: ");
    Serial.println(boompos);
    delay(d);
  }
}

void boomUp()
{
  if (servoMINboom < (boompos - 5))
  { boompos = boompos -5;
    servoboard.setPWM(2, 0, boompos);
    Serial.print("Boom position: ");
    Serial.println(boompos);
    delay(d);
  }
}

void armBackward()
{
  if (servoMAXarm > (armpos + 5))
  { armpos = armpos +5;
    servoboard.setPWM(1, 0, armpos);
    Serial.print("Arm position: ");
    Serial.println(armpos);
    delay(d);
  }
}

void armForward()
{
  if (servoMINarm < (armpos - 5))
  { armpos = armpos -5;
    servoboard.setPWM(1, 0, armpos);
    Serial.print("Arm position: ");
    Serial.println(armpos);
    delay(d);
  }
}


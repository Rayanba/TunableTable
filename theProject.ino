// Libraries ////////////////
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>
 
// Defining pins ////////////////
AccelStepper stepper(1,9,8); // Motor CLK, CW.
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD default  SCL A5 - SDA A4.
#define accelPotPin A0 // Potentiometer 1 OTA or OTB pin.
#define RoundsPotPin A1 // potentiometer 2 OTA pin.
//-----Buttons-------
#define buttonB1 11
#define buttonB2 3
#define buttonW1 A3
#define buttonW2 2
#define buttonY1 4
#define buttonY2 10
#define buttonRed 6
#define buttonLoop1 5
#define buttonLoop2 7


// Defining Vars ////////////////
int stepperMaxSpeed = 3200; // for the input limit.
int speedVal = 800; // 0 - 3200/sps.
int acceleration; // 0 - 5610.
int stepsVal = 800;
long numOfRounds; // 1 = 1600.
long fullRound = 1600; // for the rounds added.
//-----Buttons--------
bool buttonB1State = LOW;
bool buttonB2State = LOW;
bool buttonW1State = LOW;
bool buttonW2State = LOW;
bool buttonY1State = LOW;
bool buttonY2State = LOW;
bool buttonRedState = LOW;
bool buttonLoop1State = LOW;
bool buttonLoop2State = LOW;



//////////////////////////--------SETUP----------/////////////////////////
void setup()
{
  // Pins Mode
  pinMode(accelPotPin, INPUT);
  pinMode(RoundsPotPin, INPUT);
  pinMode(buttonB1, INPUT);
  pinMode(buttonB2, INPUT);
  pinMode(buttonW1, INPUT);
  pinMode(buttonW2, INPUT);
  pinMode(buttonY1, INPUT);
  pinMode(buttonY2, INPUT);
  pinMode(buttonRed, INPUT);
  pinMode(buttonLoop1, INPUT);
  pinMode(buttonLoop2, INPUT);
  // Serial Monitor
  Serial.begin(9600);

  // Stepper Default Values
  stepper.setMaxSpeed(speedVal);
  stepper.setAcceleration(1000);


  // LCD Initialization
  Wire.begin();
  lcd.init(); 
  lcd.backlight(); // activate the backlight.

  // LCD Setup
  lcd.setCursor(0, 0);
  lcd.print("SP ");
  lcd.print(speedVal);
  lcd.setCursor(9, 0);
  lcd.print("AC ");
  lcd.print(acceleration);
  lcd.setCursor(0, 1);
  lcd.print("ST ");
  lcd.print(stepsVal);
  lcd.setCursor(9, 0);
  lcd.print("RD ");
  lcd.print(numOfRounds);
}
 
 //////////////////////////--------LOOP----------/////////////////////////
void loop()
{
  // Set Vars
  acceleration= analogRead(accelPotPin)/10*30;
  stepper.setAcceleration(acceleration);
  stepper.setCurrentPosition(0);
  numOfRounds = analogRead(RoundsPotPin)/10;
  buttonB1State = digitalRead(buttonB1);
  buttonB2State = digitalRead(buttonB2);
  buttonW1State = digitalRead(buttonW1);
  buttonW2State = digitalRead(buttonW2);
  buttonY1State = digitalRead(buttonY1);
  buttonY2State = digitalRead(buttonY2);
  buttonRedState = digitalRead(buttonRed);
  buttonLoop1State = digitalRead(buttonLoop1);
  buttonLoop2State = digitalRead(buttonLoop2);

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("SP ");
  lcd.print(speedVal);
  lcd.print("  ");
  lcd.setCursor(9, 0);
  lcd.print("AC ");
  lcd.print(acceleration);
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print("ST ");
  lcd.print(stepsVal);
  lcd.print("  ");
  lcd.setCursor(9, 1);
  lcd.print("RD ");
  lcd.print(numOfRounds);
  lcd.print("  ");
  // Steps Button 1
  if (buttonB1State == HIGH){
    if (stepsVal > 0 ){
      stepsVal -= 25;
      delay(80);
    }
  }
  // Steps Button 2
  if (buttonB2State == HIGH){
    if (stepsVal < fullRound ){
      stepsVal = stepsVal + 25;
      delay(80);
    }
  }
  // Speed Button 1 
  if (buttonW1State == HIGH){
    if (speedVal > 0 ){
      speedVal -= 25;
      delay(80);
    }
  }
  // Speed Button 2 
  if (buttonW2State == HIGH){
    if (speedVal < stepperMaxSpeed){
      speedVal += 25;
      delay(80);
    }
  }
  // RunCW Button  
  if (buttonY2State == HIGH){
      long totalMove = stepsVal + (numOfRounds * fullRound);
      stepper.setMaxSpeed(speedVal);
      stepper.setAcceleration(acceleration);
      stepper.moveTo(totalMove);
  }
  // RunCCW button 
  if (buttonY1State == HIGH){
      long totalMove = stepsVal + (numOfRounds * fullRound);
      stepper.setMaxSpeed(speedVal);
      stepper.setAcceleration(acceleration);
      stepper.moveTo(-totalMove);
  }
  // Do
  while (stepper.distanceToGo() != 0 ){
    
    stepper.run();
  }
  // AjustmentCW button 
  if (buttonLoop1State == HIGH){
    long totalMove = stepsVal + (numOfRounds * fullRound);
    stepper.setMaxSpeed(speedVal);
    stepper.setAcceleration(acceleration);
    stepper.moveTo(-totalMove);
    stepper.run();
  }
  // AjustmentCCW button
  if (buttonLoop2State == HIGH){
    long totalMove = stepsVal + (numOfRounds * fullRound);
    stepper.setMaxSpeed(speedVal);
    stepper.setAcceleration(acceleration);
    stepper.moveTo(totalMove);
    stepper.run();
  }

}
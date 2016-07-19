#include <Arduino.h>
#include <Servo.h>
#include "LiquidCrystal_I2C.h"
#include <Wire.h>

/*
Front Motors. Connected to Arduino
FL - FLIN1 = D6
     FLIN2 = D5


FR - FRIN1 = D10
     FRIN2 = D11

Back motors.  Connected to Photon
BL - BLIN1 = D1
     BLIN2 = D0

BR - BRIN1 = D2
     BRIN2 = D3



*/


int FLIN1 = 6;
int FLIN2 = 5;

int FRIN1 = 11;
int FRIN2 = 10;

int ENABLE_PIN = 14;

int currentTime = 0;

int trigger = 7;
int echo = 8;

int servopin = 3;

int pos = 0;

String readString;
Servo head;


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void printLcd(String text)
{
  String text1 = text.substring(0,16);
  String text2 = text.substring(16);
  lcd.clear();
  lcd.home();
  lcd.print(text1);
  lcd.setCursor(0,1);
  lcd.print(text2);
}

void setup()
{
// head.attach(servopin);

Serial.begin(9600);
pinMode(FLIN1, OUTPUT);
pinMode(FLIN2, OUTPUT);

pinMode(trigger, OUTPUT);
pinMode(echo, INPUT);

pinMode(ENABLE_PIN, OUTPUT);

digitalWrite(ENABLE_PIN, HIGH);

lcd.begin(16,2);
lcd.backlight();
lcd.home();
lcd.print("Robot by Comp");
lcd.setCursor(0, 1);
lcd.print("Sci Major");
}





void parseCommand(String command)
{
  Serial.println(command);

  if (command.startsWith("move"))
  {
    analogWrite(FLIN1, 0);
    analogWrite(FLIN2, 0);
    analogWrite(FRIN1, 0);
    analogWrite(FRIN2, 0);

    // move 151 151
    String FLValue = command.substring(5,8);
    String FRValue = command.substring(9);
    String FLdir = String(FLValue.charAt(0));
    String FRdir = String(FRValue.charAt(0));

    // FL!
    if (FLdir == "1")
    {
      int speedvar = FLValue.substring(1).toInt() * 5;
      analogWrite(FLIN1, speedvar);
    }

    if (FLdir == "2")
    {
      int speedvar = FLValue.substring(1).toInt() * 5;
      analogWrite(FLIN2, speedvar);
    }

    if (FLdir == "0")
    {
      analogWrite(FLIN1, 0);
      analogWrite(FLIN2, 0);
    }

    // FR!

    if (FRdir == "1")
    {
      int speedvar = FRValue.substring(1).toInt() * 5;
      analogWrite(FRIN1, speedvar);
    }

    if (FRdir == "2")
    {
      int speedvar = FRValue.substring(1).toInt() * 5;
      analogWrite(FRIN2, speedvar);
    }

    if (FRdir == "0")
    {
      analogWrite(FRIN1, 0);
      analogWrite(FRIN2, 0);
    }
  }

  if (command.startsWith("print"))
  {
    printLcd(command.substring(6));
  }
  return;
}





void loop()
{

  while (Serial.available())
  {
    delay(3);
    char c = Serial.read();
    readString += c;
  }

  readString.trim();

  if (readString.length() > 0)
  {
    parseCommand(readString);

    readString = "";
  }
  //
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   head.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   head.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  //


}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int ping()
{
    long duration, inches, cm;
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigger, LOW);

    duration = pulseIn(echo, HIGH);

    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);

    return inches;
}

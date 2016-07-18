#include "application.h"




/*
Front Motors. Connected to Arduino
FL - FLIN1 = D6
     FLIN2 = D5


FR - FRIN1 = 11
     FRIN2 = 10

Back motors.  Connected to Photon
BL - BLIN1 = D1
     BLIN2 = D0

BR - BRIN1 = D2
     BRIN2 = D3



*/


int BLIN1 = D1;
int BLIN2 = D0;

int BRIN1 = D2;
int BRIN2 = D3;

String readString;


int printLcd(String text)
{
  Serial1.println("print " + text);
  return 1;
}

String makeProper(int value)
{
  String valuedata;
  if (value < 100 && value > 9)
  {
    valuedata = String("0" + String(value));
  }
  if (value < 10)
  {
    valuedata = String("00" + String(value));
  }
  else
  {
    valuedata = String(value);
  }

  return valuedata;
}

int move(String command)
{
// 151 151 151 151
String nanoCommand = command.substring(0,7);
Serial.println(nanoCommand);
String photonCommand = command.substring(8);
Serial.println(photonCommand);
delay(100);

    Serial1.println("move "+nanoCommand);
    analogWrite(BLIN1, 0);
    analogWrite(BLIN2, 0);
    analogWrite(BRIN1, 0);
    analogWrite(BRIN2, 0);

    // move 151 151
    String BLValue = photonCommand.substring(0,3);
    String BRValue = photonCommand.substring(4);
    String BLdir = String(BLValue.charAt(0));
    String BRdir = String(BRValue.charAt(0));

    // BL!
    if (BLdir == "1")
    {
      int speedvar = BLValue.substring(1).toInt() * 5;
      analogWrite(BLIN1, speedvar);
    }

    if (BLdir == "2")
    {
      int speedvar = BLValue.substring(1).toInt() * 5;
      analogWrite(BLIN2, speedvar);
    }

    if (BLdir == "0")
    {
      analogWrite(BLIN1, 0);
      analogWrite(BLIN2, 0);
    }

    // BR!

    if (BRdir == "1")
    {
      int speedvar = BRValue.substring(1).toInt() * 5;
      analogWrite(BRIN1, speedvar);
    }

    if (BRdir == "2")
    {
      int speedvar = BRValue.substring(1).toInt() * 5;
      analogWrite(BRIN2, speedvar);
    }

    if (BRdir == "0")
    {
      analogWrite(BRIN1, 0);
      analogWrite(BRIN2, 0);
    }


return 1;
}

void setup()
{
Serial1.begin(9600);
Serial.begin(9600);
pinMode(BLIN1, OUTPUT);
pinMode(BLIN2, OUTPUT);

pinMode(BRIN1, OUTPUT);
pinMode(BRIN2, OUTPUT);

Particle.function("move", move);
Particle.function("print", printLcd);
}



void parseCommand(String command)
{
if (command.startsWith("move"))
{
  move(command.substring(5));
}

if (command.startsWith("print"))
{
  printLcd(command.substring(6));
}

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
      Serial.println(readString);

      readString = "";
    }
}

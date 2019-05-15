#include <Servo.h>
/*
  Microcontroller pulses, independent linear control: An R/C servo signal is connected to terminals S1 and S2. A 1000us – 2000us pulse controls speed and direction. 1500us is stop.
*/
/*VARIABLES GENERALES*/
Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo

String a = "O";
int i;

unsigned long time1;
unsigned long timeaux;

bool start = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servoR.attach(5);
  servoL.attach(6);  // attaches the servo on pin 9 to the servo object
}


void loop() {
  // put your main code here, to run repeatedly:

  while (Serial.available()) {
    a = Serial.readString(); // read the incoming data as string
  }

  if (a.length() == 4) {
    Serial.println("vitesse max");
    avancervite();
  } else if (a.length() == 3 && a[0] == 'z') {
    avancermoyen();
    Serial.println("2nd");
  } else if (a.length() == 2 && a[0] == 'z') {
    avancerlent();
    Serial.println("1ere");
  } else if (a[0] == 'O') {
    arret();
    Serial.println("STOP");
  } else if (a[0] == 'd') {
    if (start == true) {
      tournergauche();
      start = false;
    }
    arret();

  }


  void avancervite() {
    servoL.write(40);
    servoR.write(40);
  }

  void avancermoyen() {
    servoL.write(50);
    servoR.write(50);
  }

  void avancerlent() {
    servoL.write(55);
    servoR.write(55);
  }

  void arret() {
    servoL.write(88);
    servoR.write(88);
  }

  void reculerlent() {
    servoL.write(100);
    servoR.write(100);
  }

  void reculermoyen() {
    servoL.write(110);
    servoR.write(110);
  }
  void reculervite() {
    servoL.write(130);
    servoR.write(130);
  }

  void tournergauche() {
    time1 = millis() + 2000;

    timeaux = millis();
    while (timeaux < time1) {
      servoR.write(50);
      servoL.write(110);
      timeaux = millis();
    }
  }

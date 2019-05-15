#include <Servo.h>
/*
  Microcontroller pulses, independent linear control: An R/C servo signal is connected to terminals S1 and S2. A 1000us – 2000us pulse controls speed and direction. 1500us is stop.
*/
/*VARIABLES GENERALES*/
Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo


unsigned long time1;
unsigned long timeaux;

bool start = true;

//capteur ultrason
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  servoR.attach(5, 1000, 2000);
  servoL.attach(6, 1000, 2000); // attaches the servo on pin 9 to the servo object

  /* Initialise les broches  capteur*/
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);

}

/////////////////////////////////////
void loop() {
    digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  delay(50);

//Test de commande des motors par liaison serie
  if (distance_mm > 750.0) {
    avancerMOY();
  } else if (distance_mm > 250.0) {
    avancerLENT();
  } else if (distance_mm < 250.0) {
    arret();
    if (start == true) {
      tournergauche(distance_mm);
      start = false;
    }
    arret();
    start = true;
  }

}

void arret() {
  servoL.write(90);
  servoR.write(90);
}

void avancerMAX() {
  servoL.write(35);
  servoR.write(35);
}

void avancerMOY() {
  servoL.write(55);
  servoR.write(55);
}

void avancerLENT() {
  servoL.write(75);
  servoR.write(75);
}

void reculeMAX() {
  servoL.write(145);
  servoR.write(145);
}

void reculeMOY() {
  servoL.write(125);
  servoR.write(125);
}

void reculeLENT() {
  servoL.write(105);
  servoR.write(105);
}

void tournergauche(float distance_mm) {
    time1 = millis() + 1900;

    timeaux = millis();
    while (timeaux < time1) {
      servoR.write(55);
      servoL.write(125);
      timeaux = millis();
   }
   arret();
}

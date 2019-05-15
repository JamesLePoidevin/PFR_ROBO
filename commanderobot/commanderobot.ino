#include <Servo.h>
#include <SharpIR.h>
/*
Microcontroller pulses, independent linear control: An R/C servo signal is connected to terminals S1 and S2. A 1000us – 2000us pulse controls speed and direction. 1500us is stop. 
*/
/*VARIABLES GENERALES*/
Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
bool Start =true;

/*VARIABLES CAPTEUR ULTRASON*/
/* Constantes pour les broches */
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication:
  Serial.begin(9600);

  myservo2.attach(5);
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
}

void loop(){


    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
   
  /* Affiche les résultats en mm, cm et m */
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));
   
  
     if(distance_mm < 200.0){
      arret();
     }
     else if(distance_mm < 400.0){
      avancerlent();
     }
     else{
      avancermoyen();
     }
}

void avancervite(){
  myservo.write(52);
  myservo2.write(52);
}

void avancermoyen(){
  myservo.write(62);
  myservo2.write(62);
}

void avancerlent(){
  myservo.write(72);
  myservo2.write(72);
}

void arret(){
  myservo.write(82);
  myservo2.write(82);
}

void reculerlent(){
  myservo.write(92);
  myservo2.write(92);
}

void reculermoyen(){
  myservo.write(102);
  myservo2.write(102);
}
void reculervite(){
  myservo.write(112);
  myservo2.write(112);
}

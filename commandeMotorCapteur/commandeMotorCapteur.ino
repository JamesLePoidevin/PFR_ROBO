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
bool init_E0 = true;

//capteur ultrason AVANT (CC)
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO

//capteur ultrason GAUCHE (CL)
const byte TRIGGER_PIN_CL = 4;
const byte ECHO_PIN_CL = 7;

//capteur ultrason DROITE (CR)
const byte TRIGGER_PIN_CR = 8;
const byte ECHO_PIN_CR = 9;

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/*Variables d'entree*/
bool CCT, CCL, CR, CL = false;
/*Variables de sortie*/
bool AVL, AVM, STOP, TR, TL = false;
/*Declaration des etats*/
enum Etat {E0, E1, E2, E3, E4, E5, E6};
Etat EtatPresent, EtatSuivant;

/////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  servoR.attach(5, 1000, 2000);
  servoL.attach(6, 1000, 2000); // attaches the servo on pin 9 to the servo object

  /* Initialise les broches  capteurS CC, CL, CR*/
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN_CL, OUTPUT);
  digitalWrite(TRIGGER_PIN_CL, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN_CL, INPUT);
  pinMode(TRIGGER_PIN_CR, OUTPUT);
  digitalWrite(TRIGGER_PIN_CR, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN_CR, INPUT);
}

/////////////////////////////////////
void loop() {
  digitalWrite(TRIGGER_PIN, HIGH);
  digitalWrite(TRIGGER_PIN_CL, HIGH);
  digitalWrite(TRIGGER_PIN_CR, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  digitalWrite(TRIGGER_PIN_CL, LOW);
  digitalWrite(TRIGGER_PIN_CR, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  long measure_CL = pulseIn(ECHO_PIN_CL, HIGH, MEASURE_TIMEOUT);
  long measure_CR = pulseIn(ECHO_PIN_CR, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  float distance_mm_CL = measure_CL / 2.0 * SOUND_SPEED;
  float distance_mm_CR = measure_CR / 2.0 * SOUND_SPEED;
  
  delay(50);

//Test de commande des motors par liaison serie
  /*if (distance_mm > 750.0) {
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
  }*/

  /*affectation valeur variables*/
  if(distance_mm < 750.0){
    CCL = true;
  } else{
    CCL = false;
  }
  if(distance_mm <= 250.0){
    CCT = true;
  } else{
    CCT = false;
  }
  if(distance_mm_CL <= 225.0){
    CL = true;
  } else{
    CL = false;
  }
  if(distance_mm_CR <= 225.0){
    CR = true;
  }else{
    CR = false;
  }

  /*BLOC F*/
  switch(EtatPresent){
    case E0:
      if(!CCT && !CCL){
        EtatSuivant = E1;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E1:
      if(CCL && !CCT){
        EtatSuivant = E2;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E2:
      if(CCT && !CR){
        EtatSuivant = E3;
      } else if(CCT && !CL && CR){
        EtatSuivant = E5;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E3:
      if(CL && !CCT && !CCL){
        EtatSuivant = E4;
      } else if(CL && !CCT){
        EtatSuivant = E0;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E4:
      if(!CL){
        EtatSuivant = E5;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E5:
      if(CR && !CCT && !CCL){
        EtatSuivant = E6;
      } else if(CR && !CCT){
        EtatSuivant = E0;
      } else{
        EtatSuivant = EtatPresent;
      }
    case E6:
      if(!CR){
        EtatSuivant = E3;
      } else{
        EtatSuivant = EtatPresent;
      }
  }
  
  /*BLOC M*/
  if(init_E0){
    EtatPresent = E0;
  } else{
    EtatPresent = EtatSuivant;
  }
  
  /*BLOC G*/
  if(EtatPresent == E0){
    //STOP = true;
    arret();
  }
  if(EtatPresent == E1 || EtatPresent == E4 || EtatPresent == E6){
    //AVM = true;
    avancerMOY();
  }
  if(EtatPresent == E2){
    //AVL = true;
    avancerLENT();
  }
  if(EtatPresent == E3){
    //TR = true;
    //tournerDroite
  }
  if(EtatPresent == E5){
    //TL = true;
    arret();
    if (start == true) {
      tournergauche();
      start = false;
    }
    arret();
    start = true;
  }

  init_E0 = false;
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

void tournergauche() {
    time1 = millis() + 1900;

    timeaux = millis();
    while (timeaux < time1) {
      servoR.write(55);
      servoL.write(125);
      timeaux = millis();
   }
   arret();
}

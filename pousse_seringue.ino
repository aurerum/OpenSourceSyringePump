/*Example sketch to control a stepper motor with DRV8825 stepper motor driver, AccelStepper library and Arduino: continuous rotation. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor an buttons connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

const int enable = 4;
const int BPavance = 5;
const int BPrecule = 6;
const int BPauto = 7;
const int fd1 = 8;
const int fd2 = 9;

// /!\ Please only change these two values
//----------------------------------------------------------------------------
int timeCycle = 5; // Time between each injection in minutes
float distance = 0.16; //Distance traveled for each injection in millimeters
//----------------------------------------------------------------------------

int cycle = timeCycle * 600;
float stepQuantity = (distance * 3200)/8; // On multiplie la distance voulue par le nombre de pas par tour division par le pas de ma vis


bool fd1Val;
bool fd2Val;
bool BPavanceVal;
bool BPreculeVal;
bool BPautoVal;



// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(10);


  pinMode(BPavance, INPUT);
  pinMode(BPrecule, INPUT);
  pinMode(BPauto, INPUT);
  pinMode(fd1, INPUT);
  pinMode(fd2, INPUT);
  pinMode(enable, OUTPUT);

  digitalWrite(enable, HIGH);

  Serial.begin(9600);

}

void loop() {

  
  Serial.println("arret");
  Serial.println(stepQuantity);
  Serial.println(cycle);
  digitalWrite(enable, HIGH);
  readInput();
  //Manual mode

  //Forward
  if (BPavanceVal == HIGH)//Si on appuie sur le bouton avance
    {

      while(BPavanceVal == HIGH && fd2Val == HIGH)// tant que l'on reste appuyer et qu'il n'y a pas le fin de course le moteur tourne
      {
        digitalWrite(enable, LOW);
        stepper.setSpeed(1000);
        //dirPin sens avance
        stepper.runSpeed();
        readInput();
        Serial.println("Avance");
      }
  
      
    }

    

  //Backward
  if (BPreculeVal == HIGH)
    {
      
      
     while(BPreculeVal == HIGH && fd1Val == HIGH)
     {
        digitalWrite(enable, LOW);
        stepper.setSpeed(-1000);
        //dirPin sens recule
        stepper.runSpeed();
        readInput();
        Serial.println("Recule");
     }
      
    }

 //Auto mode
   if (BPautoVal == HIGH)
    {
       
      while (fd2Val == HIGH && BPreculeVal == LOW && BPavanceVal == LOW)
      {

        for (int i=0 ; i<=300 ; i++)//i nombre d'injection d'eau, une toutes les 10 minutes couvre la nuit de 8h
        {
          readInput();
          stepper.setCurrentPosition(0);
          while(stepper.currentPosition() != stepQuantity && fd2Val == HIGH && BPreculeVal == LOW && BPavanceVal == LOW )//On avance de 1/4 de tour
            {
              digitalWrite(enable, LOW);
              stepper.setSpeed(40);
              stepper.runSpeed();
              Serial.println("auto");
              Serial.print("BP auto: ");
              Serial.println(BPautoVal);
              readInput();
            }

            digitalWrite(enable, HIGH);
            

            for (int j=0 ; j<=cycle ;j++)// On vérifie toutes les 100ms si le fdc ou un bouton n'est pas appuyé, j x delay = temps d'attente entre chaque injection
            {
              readInput();
              if (fd2Val == LOW || BPreculeVal == HIGH || BPavanceVal == HIGH)
              {
                break;
                Serial.print("break");
              }
            delay(100);
            }
            
            
        }
        break;
        readInput();

      }
    }


  

}

void readInput(){

      fd1Val = digitalRead(fd1);
      fd2Val = digitalRead(fd2);
      BPavanceVal = digitalRead(BPavance);
      BPreculeVal = digitalRead(BPrecule);
      BPautoVal = digitalRead(BPauto);

}

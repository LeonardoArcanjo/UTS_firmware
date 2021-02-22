#include "UTS150CC.h"

UTS150CC module;

//  Functions Prototypes
void setupZScan(void); 
void choiceSide(void);
void menu(void);
void iniciar(void);
void reSetup(void);

// Global Variables
long int timeStop = 0;
float distance = 0;
boolean sentido = true;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10000);

  choiceSide();
  setupZScan();

  menu();
}

void loop() {
   char opt = Serial.read();

   switch(opt){
    case 'B':
      iniciar();
      break;
    case 'S':
      reSetup();
      break;
    default:
      break;
   }
}


void setupZScan(){
  Serial.println("\nDigite o tempo (em milissegundos) em que o Top Plate ficará parado: ");
  timeStop = Serial.parseInt();
  
  if(Serial.read() == '\n'){
    Serial.print("Tempo setado: ");
    Serial.print(timeStop);
    Serial.println(" milisecs");
  }
  
  Serial.println("Digite o tamanho do passo do movimento (em milimetros): ");
  distance = Serial.parseFloat();

  if(Serial.read() == '\n'){
    Serial.print("Distancia setada: ");
    Serial.print(distance);
    Serial.println(" mm\n");
  }

  if (!sentido)
    distance = -distance;

  module.set_distance(distance);
}

void choiceSide(){
  Serial.print("Escolha uma das extremidades para comecar!!\n");
  Serial.print("Digite 'L' para posicionar o Top Plate na posicao 'Volta'\n");
  Serial.print("Ou 'R' para posicionar na posicao 'Ida'");

  char opt;
  
  while(opt != 'R' || opt != 'L'){
    if(Serial.read() == 'R'){
      module.go_EORp();
      sentido = false;
      break;
    }
    else if (Serial.read() == 'L'){
      module.go_EORm();
      sentido = true;
      break;
    }
  }
}

void menu(){
  Serial.println("\t\tZ-Scan options: ");
  Serial.println("\tB - Iniciar o Z-Scan");
  Serial.println("\tS - Reconfigurar o experimento");
}

void reSetup(){
  choiceSide();
  setupZScan();

  menu();
}

void iniciar(){
  Serial.println("Caso queira parar o experimento, digite 'P'");

  while(Serial.read() != 'P'){
    module.move_plate();
    delay(timeStop);
  }

  Serial.println("Z-Scan Parado\n");
  menu();
}

#include "UTS150CC.h"

UTS150CC module;

void show_menu(void);       // Mostra o menu de opções
char read_option(void);     // Lê a opcao desejada pelo usuario
void move_option(void);     // 

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5000);  // Seta 5 segundos para dar tempo para o usuario escrever a distancia para o Top plate se mover

  module.go_EORm();
  delay(1000);

  module.go_EORp();
  delay(1000);

  module.go_home();

  Serial.print("Pulse Counter: ");
  Serial.print(module.get_distanceRunned());
  Serial.println(" mm");

  show_menu();
}

void loop() {
   char opt = read_option();

   switch(opt){
    case 'L':
      module.go_EORm();
    break;
    case 'R':
      module.go_EORp();
    break;
    case 'H':
      module.go_home();
    break;
    case 'M':
      move_option();
    break;
   }
}

void show_menu(){
  Serial.println("Digite uma Opcao: ");
  Serial.println("Mover para Chave de fim de curso + : 'R'");
  Serial.println("Mover para Chave de fim de curso - : 'L'");
  Serial.println("Mover para Home Position - 'H'");
  Serial.println("Mover um distancia desejada: - 'M'");
}

char read_option(){
  return Serial.read();
}

void move_option(){
  Serial.print("Digite uma distancia em mm (colocar o sinal de '-' para movimentar no sentido negativo): ");
  
  float dist = Serial.parseFloat();     // Le o valor digitado pelo usuario
  
  if(Serial.read() == '\n'){
    Serial.print(dist);
    Serial.println(" mm");
  }
  module.set_distance(dist);
  module.move_plate();

  Serial.print("Distancia percorrida: ");
  Serial.print(module.get_distanceRunned());
  Serial.println(" um");
  Serial.println("===================================");
}

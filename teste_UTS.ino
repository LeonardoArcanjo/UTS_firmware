#include "UTS150CC.h"

UTS150CC module;

void setup() {
  Serial.begin(115200);

  module.go_home();

  module.set_distance(2);
  Serial.print("Distancia desejada: ");
  Serial.print(module.get_distance());
  Serial.println(" mm");
  delay(2000);

  module.move_plate(); 
  Serial.print("Pulse Count: ");
  Serial.println(module.get_pulseCount());

  module.set_distance(-2);
  Serial.print("Distancia desejada: ");
  Serial.print(module.get_distance());
  Serial.println(" mm");
  delay(1000);
  
  module.move_plate(); 
  Serial.print("Pulse Count: ");
  Serial.println(module.get_pulseCount());
}

void loop() {
  
}

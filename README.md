# UTS_firmware
Biblioteca em C/C++ referente ao dispositivo UTS150CC da Newport. 

O firmware deste Repositório é baseado no repositório Controle_UTS. 

O Dispositivo em questão é um estágio de translação de alta precisão utilizado em experimentos de óptica. 

Basicamente o módulo é constituído de uma plataforma (Top Plate) acoplado a um parafuso sem fim, que por sua vez, é acoplado a um motor DC + Encoder diferencial óptico.

O sistema (Motor + Encoder) permite que Top Plate possa se movimentar em distâncias micrométricas, da ordem de 0.1 micrômetros. 

Com isso, desenvolveu-se esse Firmware através a plataforma Arduino MEGA, devido ao microcontrolador AVR MEGA 2560, e um módulo Ponte H H L298n. Onde o microcontrolador 
ler os sinais das chaves de fim de curso e os pinos do encoder. Ao passo que a Ponte H L298n controla a velocidade do motor. 

Em breve a descrição dos métodos...

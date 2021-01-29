# UTS_firmware
Biblioteca em C/C++ referente ao dispositivo UTS150CC da Newport. 

O firmware deste Repositório é baseado no repositório Controle_UTS. 

O Dispositivo em questão é um estágio de translação de alta precisão utilizado em experimentos de óptica. 

Basicamente o módulo é constituído de uma plataforma (Top Plate) acoplado a um parafuso sem fim, que por sua vez, é acoplado a um motor DC + Encoder diferencial óptico.

O sistema (Motor + Encoder) permite que Top Plate possa se movimentar em distâncias micrométricas, da ordem de 0.1 micrômetros. 

Com isso, desenvolveu-se esse Firmware através a plataforma Arduino MEGA, devido ao microcontrolador AVR MEGA 2560, e um módulo Ponte H H L298n. Onde o microcontrolador 
ler os sinais das chaves de fim de curso e os pinos do encoder. Ao passo que a Ponte H L298n controla a velocidade do motor. 

Descrição dos métodos:

Obs: Algumas nomeclaturas são referentes ao manual do estágio, UTS Series Manual User, que se encontra no repositório.

UTS150CC(): Construtor da classe que referencia o estágio. Os pinos do arduino que estão conectados ao estágio, estão descritos no arquivo '.h'. No construtor ocorre 
a definição dos pinos como entrada, saída e ainda a ativação das interrupções.

void go_EORp(): Faz o Top Plate (ver o Manual UTS Series) se locomover até a posição onde se encontra a chave de fim de curso positiva (EOR+). 

void go_EORm(): Faz o Top Plate (ver o Manual UTS Series) se locomover até a posição onde se encontra a chave de fim de curso negativa (EOR-). 

void go_home(): Faz o Top Plate (ver o Manual UTS Series) se locomover até a posição de origem (home position) conforme o manual.

void set_distance(float d): Seta à classe a distância 'd' em milimetros que o usuário deseja que o Top Plate se locomova. Caso queria deslocar o top plate no sentido negativo 
(conforme o manual), é necessário colocar o sinal de menos '-' na frente do valor da distância desejada.

long int get_distanceRunned(): Retorna a distância contada pelo encoder, em micrômetros, percorrida pelo Top Plate. (PS: Essa classe ainda passará por melhorias pelo fato de 
seu uso parecer sem sentido ou não).

float get_distance(): Retorna o valor da última distância que o usuário setou no método set_distance();

void move_plate(): Move o Top Plate de acordo com a distância setada em set_distance();

void ai0() e ai1(): Rotinas que são chamadas quando ocorrem as interrupções. 

#ifndef _UTS150CC_H_
#define _UTS150CC_H_

#include <Arduino.h>

// Definição dos pinos utilizados Arduino Mega e conectados ao módulo
#define encA 20
#define encB 19
#define index 21
#define indexb 3
#define encAb 18
#define encBb 2
#define EORp 22		// Pino conectado ao EOR+ (ida)
#define EORm 26		// -------------- ao EOR- (volta)
#define Mech 30		// Mechanical Zero

// Definição dos pinos conectados à Ponte H
#define INA 7
#define INB 6
#define pwmPin 5

class UTS150CC{
public:
	UTS150CC();
	void go_EORp();	// Prototipo do método que faz o Top Plate ir para a posicao de EOR+ ou ida
	void go_EORm(); // ------------------------------------------------------------ EOR- ou volta
	void go_home(); // ----------------------------------------------- a home position
	
	void set_distance(float d);				// setter da variavel PRIVADA distance 
	
	long int get_distanceRunned();				// Prototipo que retorna a distancia percorida pelo Top Plate (O contador de pulsos convertido em distancia)
	float get_distance();					// retorna o valor da distancia desejada pelo usuário	(getter da variável privada distance )
	
	void move_plate(); 						// executa o movimento

	void ai0();
	void ai1();

private:
	void moveLeft(uint8_t speed);
	void moveRight(uint8_t speed);
	void stop();
	
	long int mmtoum (float pos); 		// Converte de milimetros para micrometros
	
	void enable_interrupt();			// Habilita as interrupções
	void unable_interrupt();			// Desabilita 

	volatile long int pulse_count;		// contador de pulsos gerados pelo encoder
	float distance; 					// distancia (em milimetros) desejada pelo usuario
	long int value_ref;					// numero de pulsos da distancia desejada pelo usuario
};


#endif

#include "UTS150CC.h"

#define tstBit(Y, bit) (Y & (1 << bit))		// Macro de leitura de registrador de GPIO

UTS150CC *ptrToUTCClass; // ponteiro para objetos da classe UTS150CC

// Membros estáticos que auxiliam na chamada dos métodos que serão executados quando ocorrem as interrupções
static void InterrputHandler0(void){
	ptrToUTCClass->ai0();
}

static void InterrputHandler1(void){
	ptrToUTCClass->ai1();
}

UTS150CC::UTS150CC(){							// Construtor da classe
	pinMode(EORp, INPUT);
	pinMode(EORm, INPUT);
	pinMode(Mech, INPUT);
	pinMode(index, INPUT);

	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);

	ptrToUTCClass = this;	// O ponteiro recebe o endereço do próprio objeto

	enable_interrupt(); // Ativando as interrupcoes

  	pulse_count = 0;
}

void UTS150CC::moveLeft(uint8_t speed){			// Motor girar no sentido negativo
	/*Comandos para a Ponte H que viabilizam o motor girar, no sentido NEGATIVO 
	(de acordo com o manual do UTS). A velocidade varia com o valor passado 
	como argumento em 'speed', 1 (lento / 0.04 V) - 255 (rápido / 12 V)*/

	analogWrite(pwmPin, speed);
	digitalWrite(INA, LOW);
	digitalWrite(INB, HIGH);
}

void UTS150CC::moveRight(uint8_t speed){		// Motor girar no sentido positivo
	/*Comandos para a Ponte H que viabilizam o motor girar, no sentido POSITIVO 
	(de acordo com o manual do UTS). A velocidade varia com o valor passado 
	como argumento em 'speed', 1 (lento / 0.04 V) - 255 (rápido / 12 V)*/

	analogWrite(pwmPin, speed);
	digitalWrite(INA, HIGH);
	digitalWrite(INB, LOW);
}

void UTS150CC::stop(){							// Para o motor
	// Comandos para a Ponte H PARAR o motor
	digitalWrite(INA, LOW);
	digitalWrite(INB, LOW);
}

void UTS150CC::go_EORp(){						// Desloca o Top Plate do deslocador até a posição EOR+
	/* Função para fazer o Top Plate se locomover até a extremidade onde está localizada 
	 	a chave de fim de curso '+' ou EOR+ (conforme o manual)*/

	unable_interrupt(); 		// desabilita as interrupcoes

	do{
		moveRight(255);
	} while(digitalRead(EORp) == LOW);
	stop();
}

void UTS150CC::go_EORm(){						// Desloca o Top Plate do deslocador até a posicao EOR-
	/* Função para fazer o Top Plate se locomover até a extremidade onde está localizada 
	 	a chave de fim de curso '-' ou EOR- (conforme o manual)*/

	unable_interrupt (); 		// desabilita as interrupcoes
	
	do{
		moveLeft(255);
	} while(digitalRead(EORm) == LOW);
	stop();
}

void UTS150CC::go_home(){						// Desloca o Top Plate do deslocador até a posição de Home
	/* Função para movimentar o Top Plate até a posição de 'Home'
	conforme o manual UTS.
	*/

	unable_interrupt();			// desabilita as interrupcoes

	if(digitalRead(Mech) == HIGH){		// verifica se o top plate está próximo ao EOR-
		do{
			moveRight(255);
		}while(digitalRead(Mech) == HIGH);
		stop();
		delay(10);

		do {
			moveRight(35);
		} while(!tstBit(PIND, 0));		// lê o registrador referente ao pino 21 onde está conectado o pino index
		stop();
		go_home();
	} else {							// --------------------------------------- EOR+
		do {
			moveLeft(255);
		} while (digitalRead(Mech) == LOW);
		stop();
		delay(10);
		
		do {
			moveRight(35);
		} while (!tstBit(PIND, 0));
		stop();
	}
}

void UTS150CC::enable_interrupt(){				// Habilita as Interrupções do MEGA2560
	// Ativa as interrupções nos pinos 20 e 19 em modo CHANGE (mudança de estado)
	attachInterrupt(digitalPinToInterrupt(20), InterrputHandler0, CHANGE);
	attachInterrupt(digitalPinToInterrupt(19), InterrputHandler1, CHANGE);
}

void UTS150CC::unable_interrupt(){				// Desabilita as Interrupções do MEGA2560
	// Desativa as interrupções
	detachInterrupt(digitalPinToInterrupt(20));
	detachInterrupt(digitalPinToInterrupt(19));
}

long int UTS150CC::mmtoum(float pos){ 			// Converte um valor de distância de milimmetros (mm) para Micrômetros (um)
	/*converte a distancia desejada (em mm) para micrometros (um)
	Além disso, conforme o manual, a menor medida de deslocamento é 0.3 micrômetros.
	Enquanto que a resolução do Encoder acoplado ao motor é 0.1 micrometro. 
	Sendo assim, o valor digitado em milimetros, pelo usuário, é convertido em micrometros que equivale a leitura do encoder
	*/

	return 10000 * pos;
}

void UTS150CC::set_distance(float d){			// Setter do Atributo privado 'distance' que é a distância desejada pelo usuário
	// O atributo privado 'distance' recebe o valor passado como argumento 'd' (em milimetros)
	distance = d;

	value_ref = mmtoum(distance);		// converte a distancia desejada do usuário em formato de micrometros
}

float UTS150CC::get_distance(){					// Getter do Atributo privado 'distance'
	// Getter do atributo privado distance (distancia desejada pelo usuario)
	return distance;
}

void UTS150CC::ai0(void){						// Função chamada pela ativação da interrupção conectadao ao pino Encoder A
	if(tstBit(PIND, PD3)){			// Se EncoderAb is HIGH
		if (tstBit(PINE, PE4))		// Se EncoderBb is HIGH
			pulse_count++;
		else						// Se EncoderBb is LOW
			pulse_count--;
	} else {						// Se EncoderAb is LOW
		if(tstBit(PINE, PE4))		// Se EncoderBb is HIGH
			pulse_count--;
		else						// Se EncoderBB is LOW
			pulse_count++;
	}
}

void UTS150CC::ai1(void){						// Função chamada pela ativição da interrupção conectada ao pino Encoder B
	if(tstBit(PINE, PE4)){			// EncoderBb is HIGH
		if(tstBit(PIND, PD3))		// EncoderAb is HIGH
			pulse_count--;
		else						// EncoderAb is LOW
			pulse_count++;
	} else {						// EncoderBb is LOW
		if(tstBit(PIND, PD3))		// EncoderAb is HIHG
			pulse_count++;
		else
			pulse_count--;			// EncoderAb is LOW
	}
}

long int UTS150CC::get_distanceRunned(){			// Retorna a distancia percorrida pelo Top Plate em micrometros
	// A partir da variável "pulse_count" retorna a distância percorrida (em um) pelo Top Plate 
	return pulse_count;
}

void UTS150CC::move_plate(){					// Desloca o Top plate para a distancia desejada pelo usuário 
	// compara os atributos distance e pulse_count para realizar o movimento do TOP plate
	pulse_count = 0;
	
	enable_interrupt();				// Habilita as interrupções
  
	if(value_ref > pulse_count){
		do{
			moveRight(40);
		} while((pulse_count < (value_ref - 5000)) || (tstBit(PINA, PA0)));	// Top plate moves in Right Direction while the difference between distance and pulse_count is less then 5000 OR EORp is LOW (to avoid doubt in motor DC)
		
		do{
			moveRight(37);
		} while((pulse_count < (value_ref - 1000)) || (tstBit(PINA, PA0)));

		do{
			moveRight(33);
		}while((pulse_count <= value_ref) || (tstBit(PINA, PA0)));
		
		stop(); // Stop the Top Plate moviment

	} else if (value_ref <= pulse_count){
		do{
			moveLeft(40);
		}while ((pulse_count > (value_ref + 5000)) || (tstBit(PINA, PA4)));

		do{
			moveLeft(37);
		}while ((pulse_count > (value_ref + 1000)) || (tstBit(PINA, PA4)));
		do {
			moveLeft(33);
		}while((pulse_count >= value_ref) || (tstBit(PINA, PA4)));

		stop();

	} else 
		stop();
}

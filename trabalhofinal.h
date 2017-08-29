#ifndef TRABALHOFINAL_H
#define TRABALHOFINAL_H

#ifdef AAC_RASPBERRY
#include <wiringPi.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define DHTPIN			7					// Sensor DHT11 ligado a GPIO 7
#define BOTAO_ON_PIN	1 					// Botão ligado a GPIO 1
#define LED_AMARELO		0					// LED AMARELO ligado a GPIO 0
#define LED_VERMELHO	2					// LED VERMELHO ligado a GPIO 2
#define MAXTIMINGS		85					// define usado para sensor
#define BUF_SIZE  		1024				// define buffer size para 1024

void sighandler(int signum);								// assinaturas de funções
char * nome_ficheiro();										//
char * tempo_rpi();											//
int ler_sensor(FILE* filelog, FILE* buffer, int erro);		//

#endif
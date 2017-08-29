#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE  		1024

void sighandler(int signum);
char * nome_ficheiro();
char * tempo_rpi();
int ler_sensor(FILE* filelog, FILE* buffer, int erro);

#endif
/******************************************************************************
 * FILE: debug.c
 * DESCRIPTION:
 * Versão para debug do Trabalho Final da cadeira de Arquitectura Avançada
 * de Computadores.
 *
 * Pode introduzir valores aleatórios de humidade e temperatura para debug
 * do sistema.
 *
 * Leds não funcionam.
 *
 * Para RPi e PC (com principal uso em PC).
 *
 * Para compilar, ver Makefile  
 * 
 * AUTHOR: Nuno Nelas (21502312), Catarina Silva (21401174), André Gomes (21401231)
 * LAST REVISED: 2016/02/06
 ******************************************************************************/

#include "debug.h"

char buf[BUF_SIZE];
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

int main()
{
	char *filename;
	filename = nome_ficheiro();
	int erro = 0;
	signal(SIGINT, sighandler);	
	char dir[256];

	printf("Nome do ficheiro: %s\n\n", filename);
	printf("Trabalho Final, AAC - RPi com DHT11 (MODO DEBUG)\n");
	printf("Clique no ENTER para continuar...\n\n");
	while ( getchar() != '\n');

	printf("Introduza o directório para o qual quer gravar o ficheiros\n - error.log \n - %s\n", filename);
	printf("\n Dir - ");
	scanf("%s", dir);

	while((chdir(dir) == -1) || (fopen("error.log", "wb") == 0) || (fopen(filename, "wb") == 0))
	{
        erro++;
        if (erro==120) //putz, 120 erros são bué erros
		{
			printf("Erro a criar um dos seguintes ficheiros:\n - error.log\n - %s\n\n", filename);
			printf("Tentou escrever o ficheiro %d vezes\n", erro);
        	printf("120 erros. Fechou\n");
			exit(1);
		}
	}

	if((chdir(dir) != -1) || (fopen("error.log", "wb") != 0) || (fopen(filename, "wb") != 0)) 
	{
		chdir (dir);
		FILE* filelog = fopen("error.log", "wb");
		FILE* buffer = fopen(filename, "wb");
		setvbuf(buffer, buf, _IOFBF, sizeof(buf));

		printf("\nA recolher dados...\n\n" );
		printf("-----------------------------------------\n");

		sleep(1); //mariquice

		while (1)
		{
			ler_sensor(filelog, buffer, erro);
			sleep(1); //lê a cada 1 segundo
					
			if (erro==120) //putz, 120 erros são bué erros
			{
				fclose(filelog);
				fclose(buffer);
				printf("120 erros. Fechou\n");
				exit(1);
			}
		}
	}
  	return 0;
}

void sighandler(int signum)
{
	printf("\nCTRL+C pressionado. A fechar...\n");
	exit(1);
}

char * nome_ficheiro() // função para tempo no nome do ficheiro
{
  static char filename[30];
  struct timeval tv;
  time_t timenow;

  gettimeofday(&tv, NULL);
  timenow=tv.tv_sec;

  strftime(filename,sizeof(filename),"log_%Y-%m-%d_%H-%M-%S.bin",localtime(&timenow)); //alterar para .txt para teste
  return filename;

}

char * tempo_rpi() //função para tempo no output
{
  static char time[20];
  struct timeval tv;
  time_t timenow;

  gettimeofday(&tv, NULL);
  timenow=tv.tv_sec;

  strftime(time,sizeof(time),"%Y%m%d%H%M%S",localtime(&timenow));
  return time;

}

int ler_sensor(FILE* filelog, FILE* buffer, int erro)
{
	char *tempo;
	tempo = tempo_rpi();
	int hum = 0, temp = 0;

	printf("Introduza valor para humidade: ");
	scanf("%d", &hum);
  	printf("Introduza valor para temperatura: ");
	scanf("%d", &temp);
	printf("%s, Humidade = %d %%, Temperatura = %d *C\n\n", tempo, hum, temp);
	fprintf(buffer, "%s %d %d\n", tempo, hum, temp);
  	return 0;
}
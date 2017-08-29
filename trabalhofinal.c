/******************************************************************************
 * FILE: trabalhofinal.c
 * DESCRIPTION:
 * Trabalho Final para a cadeira de Arquitectura Avançada de Computadores.
 *
 * Em modo de teste, humidade = 80 e temperatura = 22 e leds não funcionam.
 * 
 * Funciona em RPi e PC (modo teste).
 *
 * Para compilar, ver Makefile  
 * 
 * AUTHOR: Nuno Nelas (21502312), Catarina Silva (21401174), André Gomes (21401231)
 * LAST REVISED: 2016/02/06
 ******************************************************************************/

#include "trabalhofinal.h"

char buf[BUF_SIZE];
int dht11_dat[5] = { 0, 0, 0, 0, 0 }; 		//array do sensor

int main()
{
	char *filename;              			// vai buscar o nome do ficheiro "filename"
	filename = nome_ficheiro();  			// da função char * nome_ficheiro()

	int erro = 0;							// contador erros para saber quando chegar a 120

	signal(SIGINT, sighandler);				// função para fechar CTRL+C
#ifdef AAC_RASPBERRY 						// flag AAC_TRABALHO=1. modo normal do programa
	int prevButton = HIGH;

	wiringPiSetup ();						// função para inicializar GPIO com layout de WiringPi
    pinMode (BOTAO_ON_PIN, INPUT);			// configura GPIO do Botão para input
    pinMode (LED_AMARELO, OUTPUT);			// configura GPIO do LED AMARELO para output
    digitalWrite (LED_AMARELO,  LOW) ;		// desliga LED AMARELO (LOW = 0)
    pinMode (LED_VERMELHO, OUTPUT);			// configura GPIO do LED VERMELHO para output 
    digitalWrite (LED_VERMELHO, LOW) ;		// desliga LED VERMELHO (LOW = 0)
    pullUpDnControl(BOTAO_ON_PIN, PUD_UP);	// usar debounce do botão
	printf("Nome do ficheiro: %s\n\n", filename);
	printf("Trabalho Final, AAC - RPi com DHT11\n");
	printf("Clique no botão para continuar...\n\n");

    for(;;)
    {
    	digitalWrite (LED_AMARELO, HIGH) ; delay (500) ;	// liga para LED AMARELO, 500ms, desliga
    	digitalWrite (LED_AMARELO,  LOW) ; delay (500) ;	// até botão ficar a LOW (botão pressionado)

        if(prevButton == HIGH && digitalRead(BOTAO_ON_PIN) == LOW)
        {
        	prevButton = LOW; 								// muda variável para poder usar só um botão
        	digitalWrite (LED_AMARELO, LOW) ;
    
        	// para verificar se "pode entrar no directório" e "escrever/criar error.log" e "escrever/criar ficheiro.bin"
        	// valores de retorno quando dá erro:
        	//		- chdir == -1
        	//		- fopen == 0 
        	while((chdir("/media/diymount/") == -1) || (fopen("error.log", "wb") == 0) || (fopen(filename, "wb") == 0)) //para escrever e criar ficheiro "wb", para testar led "r"
        	{
        		erro++;
        		digitalWrite (LED_VERMELHO, HIGH) ; delay (500) ;
        		digitalWrite (LED_VERMELHO,  LOW) ; delay (500) ;
        		
				if (erro==120) // condição if para quando var erro chegar aos 120 erros
				{
					digitalWrite (LED_AMARELO,  LOW) ;
					digitalWrite (LED_VERMELHO, LOW) ;
					printf("Erro a criar um dos seguintes ficheiros:\n - error.log\n - %s\n\n", filename);
					printf("Tentou escrever o ficheiro %d vezes\n", erro);
					printf("120 erros. Fechou\n");
					exit(1);
				}

        		if(prevButton == LOW && digitalRead(BOTAO_ON_PIN) == LOW) // condição if para desligar botão (digitalRead(botão) == LOW (botão pressionado))
				{
					digitalWrite (LED_AMARELO,  LOW) ;
					digitalWrite (LED_VERMELHO, LOW) ;
					printf("Erro a criar um dos seguintes ficheiros:\n - error.log\n - %s\n\n", filename);
					printf("Tentou escrever o ficheiro %d vezes\n", erro);
					printf("Botão off funciona, programa fecha\n");
					exit(1);
				}
			}

			// condição if para "se tudo der bem"
			// valores diferentes de erro
			if((chdir("/media/diymount/") != -1) || (fopen("error.log", "wb") != 0) || (fopen(filename, "wb") != 0)) 
			{
				chdir ("/media/diymount/"); // entra na pasta "diymount", isto é, directoria da PEN USB
				FILE* filelog = fopen("error.log", "wb"); //cria ficheiro filelog
				FILE* buffer = fopen(filename, "wb"); // cria ficheiro buffer
				setvbuf(buffer, buf, _IOFBF, sizeof(buf)); // configura o buffer para 1024b de "espaço"

				printf( "A recolher dados...\n\n" );
				printf("-----------------------------------------\n");

				delay(1000); // mariquice 

				if ( wiringPiSetup() == -1 ) // valor de retorno do GPIO
				exit( 1 );

				while (1)
				{
					digitalWrite (LED_AMARELO,  LOW) ;
					digitalWrite (LED_VERMELHO, LOW) ;
					erro = ler_sensor(filelog, buffer, erro); //chama a função ler_sensor
					delay( 1000 ); //lê a cada 1 segundo
					
					if (erro==120)
					{
						fflush(filelog);
						fclose(filelog);
						fflush(buffer); 	// faz "flush" para a pen, buffer só esvazia para a PEN quando chega aos 1024 b
						fclose(buffer); 	// fecha ficheiro
						digitalWrite (LED_AMARELO,  LOW) ;
						digitalWrite (LED_VERMELHO, LOW) ;
						printf("120 erros. Fechou\n");
						exit(1);
					}

					if(prevButton == LOW && digitalRead(BOTAO_ON_PIN) == LOW)
					{
						fflush(filelog);
						fclose(filelog);
						fflush(buffer);
						fclose(buffer);
						digitalWrite (LED_AMARELO,  LOW) ;
						digitalWrite (LED_VERMELHO, LOW) ;
						printf("Botão off funciona, programa fecha\n");
						exit(1);
					}
				}
			}
		}	
	}
	return(0);	
#else 						// caso a flag AAC_RASPBERRY=1 não exista. "modo de teste"
	char dir[256];

	printf("Nome do ficheiro: %s\n\n", filename);
	printf("Trabalho Final, AAC - RPi com DHT11 (MODO TESTE)\n");
	printf("Clique no ENTER para continuar...\n\n");
	while ( getchar() != '\n');				// fica à espera do ENTER

	printf("Introduza o directório para o qual quer gravar o ficheiros\n - error.log \n - %s\n", filename);
	printf("\n Dir - ");
	scanf("%s", dir); 		// guarda directório inserido pelo utilizador na var dir

	while((chdir(dir) == -1) || (fopen("error.log", "wb") == 0) || (fopen(filename, "wb") == 0))
	{
        erro++;
        if (erro==120)
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

		printf( "\nA recolher dados...\n\n" );
		printf("-----------------------------------------\n");

		sleep(1);

		while (1)
		{
			ler_sensor(filelog, buffer, erro);
			sleep(1); //lê a cada 1 segundo
					
			if (erro==120)
			{
				fflush(filelog);
				fclose(filelog);
				fflush(buffer);
				fclose(buffer);
				printf("120 erros. Fechou\n");
				exit(1);
			}
		}
	}
  	return 0;
#endif
}

void sighandler(int signum) // função para fechar programas (desliga leds) com CTRL+C
{
#ifdef AAC_RASPBERRY
	digitalWrite (LED_AMARELO,  LOW) ;
	digitalWrite (LED_VERMELHO, LOW) ;
#else
	printf("\nCTRL+C pressionado. A fechar...\n");
	exit(1);
#endif
}

char * nome_ficheiro() // função para tempo no nome do ficheiro
{
  static char filename[30];
  struct timeval tv;
  time_t timenow;

  gettimeofday(&tv, NULL);
  timenow=tv.tv_sec;

  strftime(filename,sizeof(filename),"log_%Y-%m-%d_%H-%M-%S.bin",localtime(&timenow)); // função disponibilizada pelo professor
  return filename;																	   // utiliza "máscara de data" na função strftime

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
	char *tempo;						// vai buscar o nome do ficheiro "tempo"
	tempo = tempo_rpi();				// da função char * tempo_rpi()

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0; // inicializa posições da array a 0

#ifdef AAC_RASPBERRY
	uint8_t ultimoestado	= HIGH;
	uint8_t cont		= 0;
	uint8_t j		= 0, i;

	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 18 );

	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );

	pinMode( DHTPIN, INPUT );

	// detecta mudança e lê
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		cont = 0;
		while ( digitalRead( DHTPIN ) == ultimoestado )
		{
			cont++;
			delayMicroseconds( 1 );
			if ( cont == 255 )
			{
				break;
			}
		}
		ultimoestado = digitalRead( DHTPIN );

		if ( cont == 255 )
			break;

		// ignora 3 primeiros dados
		if ( (i >= 4) && (i % 2 == 0) )
		{
			//empurra cada bit para armazenar
			dht11_dat[j / 8] <<= 1;
			if ( cont > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * lê 40 bits (8bit x 5 ) + verifica o último byte
	 * imprime
	 */

	 // verifica integridade
	if ((j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))) // caso integridade OKAY
	{
		digitalWrite (LED_AMARELO, HIGH) ;
		// imprime valor para a consola para check
		printf( "%s, Humidade = %d.%d %%, Temperatura = %d.%d *C\n", tempo, dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]); 
		// fprintf retorna valores negativos caso erro
		// caso seja maior ou igual que zero, escreve para ficheiro
		if(fprintf(buffer, "%s %d %d\n", tempo, dht11_dat[0], dht11_dat[2]) >= 0)
		{
			fprintf(buffer, "%s %d %d\n", tempo, dht11_dat[0], dht11_dat[2]);
		}
		else {						// else, acende LED VERMELHO com delay de 1000ms (1s)
			erro++;
        	digitalWrite (LED_VERMELHO, HIGH) ; delay (1000) ;
        	digitalWrite (LED_VERMELHO,  LOW) ; delay (1000) ;
		}

	} else  {  // caso integridade NOT OKAY
		erro++;
		digitalWrite (LED_VERMELHO, HIGH) ;
		printf( "Integridade Corrompida\n" );
		if(fprintf(filelog, "%s %s\n", tempo, "Integridade Corrompida") >= 0)
		{
			fprintf(filelog, "%s %s\n", tempo, "Integridade Corrompida");
		}
		else {
			erro++;
        	digitalWrite (LED_VERMELHO, HIGH) ; delay (1000) ;
        	digitalWrite (LED_VERMELHO,  LOW) ; delay (1000) ;
		}
	}
	return erro;
#else
  	dht11_dat[0] = 80;			// caso não haja flag, dht11_dat[0] = 80;
  	dht11_dat[2] = 22;			//					   dht11_dat[2] = 22;
	printf("%s, Humidade = %d %%, Temperatura = %d *C\n", tempo, dht11_dat[0], dht11_dat[2]);		//imprime na consola valor de "teste"
	fprintf(buffer, "%s %d %d\n", tempo, dht11_dat[0], dht11_dat[2]);								// escreve para o buffer os dados de "teste"
  	return 0;
#endif
}
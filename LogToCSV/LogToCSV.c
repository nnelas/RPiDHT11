/******************************************************************************
 * FILE: LogToCSV.c
 * DESCRIPTION:
 * Trabalho Final para a cadeira de Arquitectura Avançada de Computadores.
 *
 * Converte ficheiro .bin em .csv.
 *
 * Para converter: ./LogToCSV nome_do_ficheiro.bin
 * 
 * Funciona em RPi e PC.
 *
 * Para compilar: gcc LogToCSV.c -o LogToCSV  
 * 
 * AUTHOR: Nuno Nelas (21502312), Catarina Silva (21401174), André Gomes (21401231)
 * LAST REVISED: 2016/02/06
 ******************************************************************************/

#include "LogToCSV.h"
 
int main(int argc, char *argv[]) {
	int i=0;
	char data[255], hum[255], temp[255], filename[255];			// var char para guardar data, humidade, temperatura e nome do ficheiro
	long cont = 0;												// var long para contador de linhas

	FILE* numLinhas = fopen(argv[1], "r");						// abre ficheiro "input" passado como argumento. utiliza para função contlinhas

	cont = contlinhas(cont, numLinhas);							// recebe contador pela função int contlinhas(long cont, FILE* numLinhas)

	FILE* bin = fopen(argv[1], "r");							// abre ficheiro "input" passado como argumento. utiliza para função main

	sscanf(argv[1], "%[^.]", filename);							// faz scan do nome do ficheiro (até .bin)
	sprintf(filename, "%s.csv", filename);						// junta filename com .csv

	FILE* csv = fopen(filename, "wb"); 							// cria ficheiro .csv com filename

	fprintf(csv, "Timestamp (YYYYMMDDhhmmss), Humidade, Temperatura\n");	// escreve para ficheiro os primeiros dados para saber cada coluna

   	while(i<cont)												
   	{
   		i++;
   		fscanf(bin, "%s", data);								// fscanf lê até espaço ou \n
   		fscanf(bin, "%s", hum);									// valores separados em cada var
   		fscanf(bin, "%s", temp);
   		fprintf(csv, "%s, %s, %s\n", data, hum, temp);			// escreve para ficheiro .csv
	}

	fclose(bin);
	fclose(csv);
}

int contlinhas(long cont, FILE* numLinhas){
	int ch;

	while ((ch = getc(numLinhas)) != EOF)						// função para contar linhas
 	{															// isto é, até ao '\n'
		if (ch == '\n') cont++;
	}

	return cont;
	fclose(numLinhas);
}
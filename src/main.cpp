/*
 * main.cpp
 *
 *  Created on: 11 de abril de 2019
 *      Author: pedro
 */

#include "Arduino.h"

#include <Ultrasonic.h> // Biblioteca ara sensor ultrassônico

#define TAMANHO_JANELA 10 // Tamanho da janela de valores usados para média

#define INTERVALO_LEITURA_SENSORES 100 // Intervalo de leitura dos sensores em ms

/* Pinos para sensores */
#define US1_TRIGGER 4 // Trigger sensor ultrassônico 1
#define US1_ECHO    5 // Echo sensor ultrassônico 1
#define US2_TRIGGER 6 // Trigger sensor ultrassônico 2
#define US2_ECHO    7 // Echo sensor ultrassônico 2
#define PRESENCA    8 // Pino para sensor de presença

/* Instanciando sensores ultrassônicos */
Ultrasonic us1(US1_TRIGGER, US1_ECHO);
Ultrasonic us2(US2_TRIGGER, US2_ECHO);

/* Variáveis para leitura do sensor ultrassônico 1 */
float leiturasUs1[TAMANHO_JANELA];
int indiceUs1;

float mediaVetor(float *vetor) {
	float menor, maior;       // Variáveis para descarte
	int indiceJanelaCircular; // Índice onde se inicia a janela deslizante (circular)
	if (indiceUs1 == 0) {
		indiceJanelaCircular = TAMANHO_JANELA - 1;
	}
	else {
		indiceJanelaCircular = indiceUs1 - 1;
	}
	menor = vetor[indiceJanelaCircular];
	maior = vetor[indiceJanelaCircular];

	float valor;          // Valor do vetor no índice atual
	float soma = 0;       // Soma de todos os valores do vetor
	int indiceDeslizante; // Índice para percorrer a janela deslizante
	for (int i = 0; i < TAMANHO_JANELA; i++) {
		indiceDeslizante = i + indiceJanelaCircular;
		if (indiceDeslizante >= TAMANHO_JANELA) {
			valor = vetor[indiceDeslizante - TAMANHO_JANELA];
		}
		else {
			valor = vetor[indiceDeslizante];
		}

		/* Atualiza os valores segundo o valor atualmente lido do vetor */
		soma += valor;
		if (valor > maior)
			maior = valor;
		if (valor < menor)
			menor = valor;
	}

	/* Descarta os valores maiores e menores */
	soma -= maior;
	soma -= menor;

	/* Retorna a média dos valores descartando o maior e o menor valor */
	return soma / (TAMANHO_JANELA - 2);
}

void setup(){
  Serial.begin(9600);
  indiceUs1 = 0;
}

void loop(){
  float cmMsec;
  long microsec = us1.timing();

  cmMsec = us1.convert(microsec, Ultrasonic::CM);

  leiturasUs1[indiceUs1] = cmMsec;

  /* Incrementa e corrige índice da janela deslizante */
  indiceUs1++;
  if (indiceUs1 >= TAMANHO_JANELA) {
	  indiceUs1 = 0;
  }

  /* Calcula e mostra a média no terminal */
  float media = mediaVetor(leiturasUs1);
  Serial.println(media);

  delay(INTERVALO_LEITURA_SENSORES);
}

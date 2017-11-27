#include "processo.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <queue>

using namespace std;

int TEMPO = 0;
int MEMORIA_PRINCIPAL[100];
int memoria_utilizada = 0;
int burst = 2;

typedef queue<Processo> Fila;  // FILA DE PROCESSOS

struct Bloqueado {
  // Lista de processos;
};

struct CPU {
  Processo *processo;
};

struct Pronto {
  Fila processo;
};

// Bloqueado bloqueado;
Pronto pronto;
CPU cpu;

void RUN() {    // ERRO DE LIXO NA CPU

  if ( cpu.processo != NULL && cpu.processo->recursos.burst != 0 ) {
    cpu.processo->recursos.burst--;
    if ( cpu.processo->recursos.burst == 0 ) {  //ADD
      memoria_utilizada = memoria_utilizada - cpu.processo->memoria.tamanho;
      for (int i = cpu.processo->memoria.inicio; i <= cpu.processo->memoria.fim; i++) // LIBERO DA MEMORIA PRINCIPAL
        MEMORIA_PRINCIPAL[i] = 0;
      cpu.processo = NULL;  //ADD
    }
  }
  else
    cpu.processo = NULL;

  if ( cpu.processo == NULL && !pronto.processo.empty() ) {
    cpu.processo = &pronto.processo.front();
    pronto.processo.pop();
  }
}

bool espaco_em_memoria(int *inicio, int *fim, int t) {
  int *p = NULL;
  int tamanho = 0;
  int i, f;

  for (int x=0; x < 100; x++){
    if (m[x] == 0){
      i = x;
      while(m[x] == 0){
        tamanho++;
        if ( x < 99 )
          x++;
        else
          break;
      }
      if(tamanho >= t){
        f = (i + t) - 1;

        *inicio = i;
        *fim = f;
        return true;
      }
      tamanho = 0;
    }
  }
  return false;
}

void novo_processo() {
  int t, i = 0, f = 0;
  t = 2 + (rand() % 8);    // PROCESSOS DE 2K ATÉ 9K

  if ( memoria_utilizada+t < 100 && espaco_em_memoria(&i, &f, t) ) {
      Processo p;
      p.recursos.PID = novo_PID();
      p.recursos.burst = burst;
      p.recursos.quantum = 2;
      p.memoria.inicio = i;
      p.memoria.fim = f;
      p.memoria.tamanho = t;
      memoria_utilizada = memoria_utilizada + t;
      pronto.processo.push(p);
      for (int x = i; x <= f; x++)
        MEMORIA_PRINCIPAL[x] = p.recursos.PID;
    }
    else
      cout << "Memoria Principal sem espaco para " << t << "K\b\b\n\n";
}

void ENTRADA() {
  char c;
  cout << endl << endl;
  system ("/bin/stty raw");       // INPUT
  c = getchar();                  // SEM
  system ("/bin/stty cooked");    // ENTER
  system("clear");
  if ( c == ' ' )
    novo_processo();
}

void PROCESSOS_AGUARDANDO(Fila *f) {
  Fila d;
  cout << "PRONTO:\t\t";
  if ( f->empty() )
    cout << "VAZIO";
  while (!f->empty()) {
    cout << "[" << f->front().memoria.tamanho << "K] ";
    d.push( f->front() );
    f->pop();
  }
  *f = d;
}

void MONITOR_MEMORIA() {
  cout << endl << "MEMORIA:\t" << memoria_utilizada << "%" << endl << endl;
  cout << "\t\t[";
  for (int x = 0; x < 100; x++) {
    if ( MEMORIA_PRINCIPAL[x] != 0 )
      cout << "|";
    else
      cout << "-";
  }
  cout << "]" << endl << endl;
}

void MONITOR_CPU() {
  if ( cpu.processo != NULL )
    cout << "CPU: \t\t[ " << cpu.processo->memoria.tamanho << "K ] [ " << cpu.processo->recursos.burst << " ]" << endl;
  else
    cout << "CPU: \t\t[ OSCIOSO ]" << endl;
}

int main() {
  for (int i = 0; i < 100; i++)
    MEMORIA_PRINCIPAL[i] = 0;

  cout << "Burst por processo: ";
  cin >> burst;
  system("clear");

  cout << "[ENTER]  para simular o tempo." << endl;
  cout << "[ESPACO] para chegar um processo." << endl;

  while (true) {
    TEMPO++;                // TEMPORIZADOR
    ENTRADA();              // +PROCESSO ?
    RUN();                  // CPU
    MONITOR_CPU();          // MONITORA CPU, OSCIOSA OU NAO ?
    MONITOR_MEMORIA();
    PROCESSOS_AGUARDANDO(&pronto.processo); // FILA DE PRONTO
  }

  return 0;
}

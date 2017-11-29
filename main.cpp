#include "processo.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <list> // http://www.cplusplus.com/reference/list/list/

using namespace std;

int TEMPO = 0;
int MEMORIA_PRINCIPAL[100];
int memoria_utilizada = 0;

typedef queue<Processo> Fila;  // FILA DE PROCESSOS
typedef list<Processo> Lista;  // LISTA DE PROCESSOS

struct Bloqueado {
  Lista processo;
  int timer;
};
Bloqueado bloqueado;

struct CPU {
  Processo *processo;
};
CPU cpu;

struct Pronto {
  Fila processo;
};
Pronto pronto;

void RUN() {
  int bloq = (rand() % 7);  // PROBABILIDADE PARA ESTADO DE ESPERA

  if ( cpu.processo != NULL && cpu.processo->recursos.quantum > 0 ) {                 // SE CPU ESTIVER EM USO
    cpu.processo->recursos.quantum--;
    cpu.processo->recursos.burst--;

    if ( cpu.processo->recursos.burst <= 0 ) {                                        // SE O PROCESSO TERMINAR
      for (int i = cpu.processo->memoria.inicio; i <= cpu.processo->memoria.fim; i++) // LIBERO DA MEMORIA PRINCIPAL
        MEMORIA_PRINCIPAL[i] = 0;
      memoria_utilizada = memoria_utilizada - cpu.processo->memoria.tamanho;          // ATUALIZA O USO DA MEMORIA
      cpu.processo = NULL;                                                            // CPU LIVRE
    }
    else if ( cpu.processo->recursos.quantum <= 0 ) {                                 // PREEMPÇÃO
      cpu.processo->recursos.quantum = 5;                                             //
      pronto.processo.push( *cpu.processo );                                          // REENVIA O PROCESSO PARA A FILA DE PRONTO
      cpu.processo = NULL;                                                            // LIBERA O PROCESSADOR
    }
    else if ( bloq == 1 ) {                                                           // CONDIÇÃO PARA ESTADO DE ESPERA
      cpu.processo->recursos.timer = TEMPO;                                           // SALVA O TEMPO ATUAL
      cpu.processo->recursos.espera = 5 + (rand() % 10);                              // TEMPO DE ESPERA
      bloqueado.processo.push_front( *cpu.processo );                                 // MANDA DO CPU PARA LISTA DO ESTADO DE ESPERA
      cpu.processo = NULL;                                                            // LIBERA O PROCESSADOR
    }

  }
  else
    cpu.processo = NULL;

  if ( cpu.processo == NULL && !pronto.processo.empty() ) {                           // SE HOUVER PROCESSOS PARA EXECUTAR NA FILA DE PRONTO
    cpu.processo = &pronto.processo.front();                                          // MANDA DA FILA DE PRONTO PARA CPU
    pronto.processo.pop();                                                            //
  }
}

bool espaco_em_memoria(int *inicio, int *fim, int t) {
  int *p = NULL;
  int tamanho = 0;
  int i, f;

  for (int x=0; x < 100; x++){
    if (MEMORIA_PRINCIPAL[x] == 0){
      i = x;
      while(MEMORIA_PRINCIPAL[x] == 0){
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
  int i = 0, f = 0;
  int burst = (2+(rand()%3)) * (4+(rand()%6)); // DE 8 a 50 BURST
  int t = 2 + (rand() % 8);    // PROCESSOS DE 2K ATÉ 9K

  if ( memoria_utilizada+t <= 100 && espaco_em_memoria(&i, &f, t) ) {
      Processo p;
      p.recursos.PID = novo_PID();
      p.recursos.burst = burst;
      p.recursos.quantum = 5;
      p.memoria.inicio = i;
      p.memoria.fim = f;
      p.memoria.tamanho = t;
      memoria_utilizada = memoria_utilizada + t;
      pronto.processo.push(p);
      for (int x = i; x <= f; x++)
        MEMORIA_PRINCIPAL[x] = p.recursos.PID;
      cout << "Novo processo: \t" << t << "K"  << endl;
      cout << "Burst: \t\t" << burst << endl << endl;
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
  if ( cpu.processo != NULL ) {
    cout << "\t\t <ID>\t<Burst>\t<Quantum>" << endl;
    cout << "CPU: \t\t[ " << cpu.processo->memoria.tamanho << "K ]\t [ " << cpu.processo->recursos.burst << " ]\t  [ " << cpu.processo->recursos.quantum << " ]" << endl;
  }
  else
    cout << "CPU: \t\t[ OSCIOSO ]" << endl;
}

void IO() {
  Lista aux;
  cout << endl << endl << "I/O:" << endl;
  while ( !bloqueado.processo.empty() ) {                       // ENQUANTO TEM PROCESSOS EM ESTADO DE ESPERA
    cout << "[ " << bloqueado.processo.front().memoria.tamanho << "K ] [ " << bloqueado.processo.front().recursos.espera << " ]" << endl;
    bloqueado.processo.front().recursos.espera--;
    if ( bloqueado.processo.front().recursos.espera <= 0 ) {    // SE ACABOU O TEMPO DE ESPERA
      pronto.processo.push( bloqueado.processo.front() );       // MANDA DO ESTADO DE ESPERA PARA PRONTO
      bloqueado.processo.pop_front();                           //
    }
    else {                                                      // CONTINUA MOSTRANDO O RESTANTE COM UMA LISTA AUXILIAR
      aux.push_front( bloqueado.processo.front() );             //
      bloqueado.processo.pop_front();                           //
    }
  }

  while ( !aux.empty() ) {                                      // RETORNA DA LISTA AUXILIAR PARA A LISTA DE ESPERA ORIGINAL
    bloqueado.processo.push_front( aux.front() );               //
    aux.pop_front();                                            //
  }

}

int main() {
  for (int i = 0; i < 100; i++) // INICIALIZA COM A MEMORIA VAZIA
    MEMORIA_PRINCIPAL[i] = 0;

  cout << "[ENTER]  | para simular o tempo." << endl;
  cout << "[ESPACO] | para chegar um processo." << endl;

  while (true) {
    TEMPO++;                // TEMPORIZADOR
    ENTRADA();              // +PROCESSO ?
    RUN();                  // CPU
    MONITOR_CPU();          // MONITORA CPU, OSCIOSA OU NAO ?
    MONITOR_MEMORIA();      // MOSTRA OS PROCESSOS ALOCADOS A MEMORIA
    PROCESSOS_AGUARDANDO(&pronto.processo); // FILA DE PRONTO
    IO();                   // ENTRADA E SAIDA
  }

  return 0;
}

#include <stdlib.h>
#include <string>

int id_PID = 2346;

int novo_PID() {
  id_PID = id_PID + 277;
  return id_PID;
}

struct Recursos {
  std::string nome;
  int PID;
  int quantum;
  int burst;
  int espera;
  int timer;
  // OUTROS
};

struct Registradores {
  // REGISTRADORES GERAIS
  // REGISTRADOR PC
  // REGISTRADOR SP
  // REGISTRADOR DE STATUS
};

struct Memoria {
  // PONTEIROS DA RAM ALOCADO
  int tamanho;
  int inicio;
  int fim;
};

struct Processo {
  // Registradores   registradores;
  Recursos        recursos;
  Memoria         memoria;
};

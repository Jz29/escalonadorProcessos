#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
// #include "processo.h"

using namespace std;

// typedef list<Processo> Lista;  // LISTA DE PROCESSOS

int main(){

  // Lista l;
  list<int> l;
  // Processo p;

  l.push_front(10);
  l.push_front(11);
  l.push_front(12);

  l.remove(16);

  cout << l.front() << '\n';

  return 0;
}

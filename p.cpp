#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void memoria(int *m) {
  for (int x = 0; x < 100; x++) {
    if ( m[x] == 0 )
      cout << "-";
    else
      cout << "|";
  }
  cout << endl << endl;
}

bool b(int *m, int t, int *inicio, int *fim){
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

int main(){

  int *v;
  int m[100];

  for (int x=0; x < 100; x++)
    m[x] = 1;

  m[1] = 0;

  m[2] = 0;
  m[3] = 0;

  m[4] = 0;
  m[5] = 0;

  m[73] = 0;

  m[90] = 0;
  m[91] = 0;
  m[92] = 0;
  // m[93] = 0;

  m[95] = 0;
  m[96] = 0;
  m[97] = 0;
  m[98] = 0;
  m[99] = 0;

  int t, i, f;
  cout << "tamanho: ";
  cin >> t;

  if ( b(m, t, &i, &f) )
    cout << i << endl << f << endl;

  memoria(m);

  return 0;
}

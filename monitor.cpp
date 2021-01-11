#include "monitor.h"

#include <iostream> 
#include <stdlib.h> 

using namespace std; 

pthread_mutex_t mutex;

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    return;
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;
    return;
}

void Monitor::verificar() {
    return;
}
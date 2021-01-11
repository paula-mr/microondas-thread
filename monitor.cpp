#include "monitor.h"

#include <iostream> 
#include <stdlib.h> 

using namespace std; 

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
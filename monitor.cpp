#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <map>
#include <stdlib.h> 

using namespace std; 

map<const char*, Personagem> fila;

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    fila.insert({p.name, p});

    pthread_mutex_lock(&this->mutex);

    if ((p.equals(HOWARD) || p.equals(BERNADETTE)) && fila.find(SHELDON) != fila.end()) {
        cout << p.name << " esperando Sheldon terminar de usar o forno" << endl; 
        pthread_cond_wait(&this->sheldonDoesNotWantToUse, &this->mutex);
        cout << "Sheldon liberou o forno para Howard" << endl; 
    }

}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;
    if (p.equals(SHELDON)) {
        pthread_cond_signal(&this->sheldonDoesNotWantToUse);
    }
    fila.erase(p.name);
    pthread_mutex_unlock(&this->mutex);
}

void Monitor::verificar() {
    return;
}
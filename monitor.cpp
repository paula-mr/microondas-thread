#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <set>
#include <stdlib.h> 
#include <string>

using namespace std; 

set<string> fila;

bool stuartDeveEsperar() {
    if (fila.size() == 1 && *fila.begin() == KRIPKE) return false;
    return true;
}

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    fila.insert(p.name);

    pthread_mutex_lock(&this->mutex);

    cout << p.name << " has lock" << endl;

    if (p.equals(HOWARD) || p.equals(BERNADETTE)) {
        esperarPorSheldonEAmy(); 
    } else if (p.equals(LEONARD) || p.equals(PENNY)) {
        esperarPorHowardEBernadette();
    } else if (p.equals(SHELDON) || p.equals(AMY)) {
        esperarPorLeonardEPenny();
    } else if (p.equals(STUART)) {
        esperarPorSheldonEAmy(); 
        esperarPorHowardEBernadette();
        esperarPorLeonardEPenny();
    } else if (p.equals(KRIPKE)) {
        esperarPorSheldonEAmy(); 
        esperarPorHowardEBernadette();
        esperarPorLeonardEPenny();
        esperarPorStuart();
    }
}

void Monitor::esperarPorSheldonEAmy() {
    if (fila.find(SHELDON) != fila.end() || fila.find(AMY) != fila.end()) {
        cout << "Esperando Sheldon terminar de usar o forno" << endl; 
        pthread_cond_wait(&this->sheldonAndAmyDontWantToUse, &this->mutex);
        cout << "Sheldon liberou o forno para uso" << endl;
    }
}

void Monitor::esperarPorHowardEBernadette() {
    if (fila.find(HOWARD) != fila.end() || fila.find(BERNADETTE) != fila.end()) {
        cout << "Esperando Howard terminar de usar o forno" << endl; 
        pthread_cond_wait(&this->howardAndBernadetteDontWantToUse, &this->mutex);
        cout << "Howard liberou o forno para uso" << endl; 
    }
}

void Monitor::esperarPorLeonardEPenny() {
    if (fila.find(LEONARD) != fila.end() || fila.find(PENNY) != fila.end()) {
        cout << "Esperando Leonard terminar de usar o forno" << endl; 
        pthread_cond_wait(&this->leonardAndPennyDontWantToUse, &this->mutex);
        cout << "Leonard liberou o forno para uso" << endl;
    }
}

void Monitor::esperarPorStuart() {
    if (fila.find(STUART) != fila.end()) {
        cout << "Esperando Stuart terminar de usar o forno" << endl; 
        pthread_cond_wait(&this->stuartDoesntWantToUse, &this->mutex);
        cout << "Stuart liberou o forno para uso" << endl;
    }
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;

    fila.erase(p.name);
    pthread_mutex_unlock(&this->mutex);

    if (p.equals(SHELDON) || p.equals(AMY)) {
        pthread_cond_signal(&this->sheldonAndAmyDontWantToUse);
    } else if (p.equals(HOWARD) || p.equals(BERNADETTE)) {
        pthread_cond_signal(&this->howardAndBernadetteDontWantToUse);
    } else if (p.equals(LEONARD) || p.equals(PENNY)) {
        pthread_cond_signal(&this->leonardAndPennyDontWantToUse);
    } else if (p.equals(STUART)) {
        pthread_cond_signal(&this->stuartDoesntWantToUse);
    }
}

void Monitor::verificar() {
    return;
}
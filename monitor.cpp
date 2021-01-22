#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <set>
#include <stdlib.h> 
#include <string>

using namespace std; 

set<string> fila;

bool stuartDeveEsperar() {
    return fila.size() != 1 || *fila.begin() != KRIPKE;
}

bool casalCompleto(string casal1, string casal2) {
    return fila.find(casal1) != fila.end() && fila.find(casal2) != fila.end();
}

bool nenhumMembroDoCasal(string casal1, string casal2) {
    return fila.find(casal1) == fila.end() && fila.find(casal2) == fila.end();
}

void signalCond(pthread_cond_t* cond) {
    if (pthread_cond_signal(cond) != 0) {
        perror("pthread_cond_signal() error");
        exit(2);
    }
}

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    fila.insert(p.name);

    if (pthread_mutex_lock(&this->mutex) != 0) {
        perror("pthread_mutex_lock error");
        exit(2);
    }

    cout << p.name << " has lock" << endl;

    if (p.equals(HOWARD) || p.equals(BERNADETTE)) {
        esperarPorSheldonEAmy();
        casalHowardEBernadetteAtivo = casalCompleto(HOWARD, BERNADETTE);
        if (!casalHowardEBernadetteAtivo) { 
            esperarPorLeonardEPenny();
            esperarPorSheldonOuAmy(); 
        }
    } else if (p.equals(LEONARD) || p.equals(PENNY)) {
        esperarPorHowardEBernadette();
        casalLeonardEPennyAtivo = casalCompleto(LEONARD, PENNY);
        if (!casalLeonardEPennyAtivo) {
            esperarPorSheldonEAmy(); 
            esperarPorHowardOuBernadette(); 
        }
    } else if (p.equals(SHELDON) || p.equals(AMY)) {
        esperarPorLeonardEPenny();
        casalSheldonEAmyAtivo = casalCompleto(SHELDON, AMY);
        if (!casalSheldonEAmyAtivo) {
            esperarPorHowardEBernadette(); 
            esperarPorLeonardOuPenny(); 
        }
    } else if (p.equals(STUART)) {
        esperarPorSheldonEAmy(); 
        esperarPorHowardEBernadette();
        esperarPorLeonardEPenny();

        esperarPorSheldonOuAmy(); 
        esperarPorHowardOuBernadette();
        esperarPorLeonardOuPenny();
    } else if (p.equals(KRIPKE)) {
        esperarPorSheldonEAmy(); 
        esperarPorHowardEBernadette();
        esperarPorLeonardEPenny();

        esperarPorSheldonOuAmy(); 
        esperarPorHowardOuBernadette();
        esperarPorLeonardOuPenny();
        esperarPorStuart();
    }
}

void Monitor::esperarPorSheldonEAmy() {
    if (casalSheldonEAmyAtivo) {
        cout << "Esperando Sheldon e Amy terminarem de usar o forno" << endl; 
        if (pthread_cond_wait(&this->sheldonAndAmyAreNotTogether, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Sheldon e Amy liberaram o forno para uso" << endl;
    }
}

void Monitor::esperarPorHowardEBernadette() {
    if (casalHowardEBernadetteAtivo) {
        cout << "Esperando Howard e Bernadette terminarem de usar o forno" << endl; 
        if (pthread_cond_wait(&this->howardAndBernadetteAreNotTogether, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Howard e Bernadette liberaram o forno para uso" << endl;
    }
}

void Monitor::esperarPorLeonardEPenny() {
    if (casalLeonardEPennyAtivo) {
        cout << "Esperando Leonard e Penny terminarem de usar o forno" << endl; 
        if (pthread_cond_wait(&this->leonardAndPennyAreNotTogether, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Leonard e Penny liberaram o forno para uso" << endl;
    }
}

void Monitor::esperarPorSheldonOuAmy() {
    if (fila.find(SHELDON) != fila.end() || fila.find(AMY) != fila.end()) {
        cout << "Esperando Sheldon terminar de usar o forno" << endl; 
        if (pthread_cond_wait(&this->sheldonAndAmyDontWantToUse, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Sheldon liberou o forno para uso" << endl;
    }
}

void Monitor::esperarPorHowardOuBernadette() {
    if (fila.find(HOWARD) != fila.end() || fila.find(BERNADETTE) != fila.end()) {
        cout << "Esperando Howard terminar de usar o forno" << endl; 
        if (pthread_cond_wait(&this->howardAndBernadetteDontWantToUse, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Howard liberou o forno para uso" << endl; 
    }
}

void Monitor::esperarPorLeonardOuPenny() {
    if (fila.find(LEONARD) != fila.end() || fila.find(PENNY) != fila.end()) {
        cout << "Esperando Leonard terminar de usar o forno" << endl; 
        if (pthread_cond_wait(&this->leonardAndPennyDontWantToUse, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Leonard liberou o forno para uso" << endl;
    }
}

void Monitor::esperarPorStuart() {
    if (fila.find(STUART) != fila.end()) {
        cout << "Esperando Stuart terminar de usar o forno" << endl; 
        if (pthread_cond_wait(&this->stuartDoesntWantToUse, &this->mutex) != 0) {
            perror("pthread_cond_wait() error");
            exit(2);
        }
        cout << "Stuart liberou o forno para uso" << endl;
    }
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;

    fila.erase(p.name);
    pthread_mutex_unlock(&this->mutex);

    if (p.equals(SHELDON)) {
        
    }

    if (p.equals(SHELDON) || p.equals(AMY)) {
        signalCond(&this->sheldonAndAmyDontWantToUse);
        if (casalCompleto(SHELDON, AMY)) {
            signalCond(&this->sheldonAndAmyAreNotTogether);
        }
        if (nenhumMembroDoCasal(SHELDON, AMY)) {
            casalSheldonEAmyAtivo = false;
        }
    } else if (p.equals(HOWARD) || p.equals(BERNADETTE)) {
        signalCond(&this->howardAndBernadetteDontWantToUse);
        if (casalCompleto(HOWARD, BERNADETTE)) {
            signalCond(&this->howardAndBernadetteAreNotTogether);
        }
        if (nenhumMembroDoCasal(HOWARD, BERNADETTE)) {
            casalHowardEBernadetteAtivo = false;
        }
    } else if (p.equals(LEONARD) || p.equals(PENNY)) {
        signalCond(&this->leonardAndPennyDontWantToUse);
        if (casalCompleto(LEONARD, PENNY)) {
            signalCond(&this->leonardAndPennyAreNotTogether);
        }
        if (nenhumMembroDoCasal(LEONARD, PENNY)) {
            casalLeonardEPennyAtivo = false;
        }
    } else if (p.equals(STUART)) {
        signalCond(&this->stuartDoesntWantToUse);
    }
}

void Monitor::verificar() {
    return;
}
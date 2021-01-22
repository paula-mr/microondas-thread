#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <map>
#include <stdlib.h> 
#include <string>

using namespace std; 

map<string, int> lista;


bool estaPresente(string personagem) {
    return lista.find(personagem) != lista.end();
}

string encontrarPrimeiro(string p1, string p2) {
    for (auto i: lista) {
        cout << "LISTA " << i.first << endl;
        if (i.first == p1) {
            return p1;
        } else if (i.first == p2){
            return p2;
        }
    }
    return "";
}

bool casalCompleto(string casal1, string casal2) {
    return lista.find(casal1) != lista.end() && lista.find(casal2) != lista.end();
}

bool nenhumMembroDoCasal(string casal1, string casal2) {
    return lista.find(casal1) == lista.end() && lista.find(casal2) == lista.end();
}

void signalCond(pthread_cond_t* cond) {
    if (pthread_cond_signal(cond) != 0) {
        perror("pthread_cond_signal() error");
        exit(2);
    }
}

void waitCond(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    if (pthread_cond_wait(cond, mutex) != 0) {
        perror("pthread_cond_wait() error");
        exit(2);
    }
}

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    lista.insert({p.name, ordem});
    ordem++;

    if (pthread_mutex_lock(&this->mutex) != 0) {
        perror("pthread_mutex_lock error");
        exit(2);
    }

    cout << p.name << " has lock" << endl;

    if (p.equals(HOWARD)) {
        if (lista.size() > 1) waitCond(&howardLiberado, &mutex);
    } else if (p.equals(BERNADETTE)) {
        if (lista.size() > 1) waitCond(&bernadetteLiberada, &mutex);
    } else if (p.equals(LEONARD)) {
        if (lista.size() > 1) waitCond(&leonardLiberado, &mutex);
    } else if (p.equals(PENNY)) {
        if (lista.size() > 1) waitCond(&pennyLiberada, &mutex);
    } else if (p.equals(SHELDON)) {
        if (lista.size() > 1) waitCond(&sheldonLiberado, &mutex);
    } else if (p.equals(AMY)) {
        if (lista.size() > 1) waitCond(&amyLiberada, &mutex);
    } else if (p.equals(STUART)) {
        if (lista.size() > 1) waitCond(&stuartLiberado, &mutex);
    } else if (p.equals(KRIPKE)) {
        if (lista.size() > 1) waitCond(&kripkeLiberado, &mutex);
    }
}

bool deveriaExecutarCasal(string c1, string c2, string ultimoExecutado) {
    return (casalCompleto(SHELDON, AMY) || ((estaPresente(c1) || estaPresente(c2)) &&
        (ultimoExecutado == c1 || ultimoExecutado == c2)));
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;

    lista.erase(p.name);
    pthread_mutex_unlock(&this->mutex);

    if (deveriaExecutarCasal(SHELDON, AMY, p.name) && !casalCompleto(LEONARD, PENNY)) {
        cout << "CASAL S A" << endl;
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            signalCond(&sheldonLiberado);
        } else {
            signalCond(&amyLiberada);
        }
    } else if (deveriaExecutarCasal(LEONARD, PENNY, p.name) && !casalCompleto(HOWARD, BERNADETTE)) {
        cout << "CASAL L P" << endl;
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            signalCond(&leonardLiberado);
        } else {
            signalCond(&pennyLiberada);
        }
    } else if (deveriaExecutarCasal(HOWARD, BERNADETTE, p.name) && !casalCompleto(SHELDON, AMY)) {
        cout << "CASAL H B" << endl;
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            cout << "Sig h" << endl;
            signalCond(&howardLiberado);
        } else {
            cout << "Sig b" << endl;
            signalCond(&bernadetteLiberada);
        }
    } else if ((estaPresente(SHELDON) || estaPresente(AMY)) 
        && (!estaPresente(LEONARD) && !estaPresente(PENNY))) {
        cout << "S o A" << endl;
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            signalCond(&sheldonLiberado);
        } else {
            signalCond(&amyLiberada);
        }
    } else if ((estaPresente(HOWARD) || estaPresente(BERNADETTE)) 
        && (!estaPresente(SHELDON) && !estaPresente(AMY))) {
        cout << "H o B" << endl;
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            signalCond(&howardLiberado);
        } else {
            signalCond(&bernadetteLiberada);
        }
    } else if ((estaPresente(LEONARD) || estaPresente(PENNY)) 
        && (!estaPresente(HOWARD) && !estaPresente(BERNADETTE))) {
        cout << "L o P" << endl;
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            signalCond(&leonardLiberado);
        } else {
            signalCond(&pennyLiberada);
        }
    } else if (!estaPresente(LEONARD) && !estaPresente(PENNY) &&
                !estaPresente(HOWARD) && !estaPresente(BERNADETTE) &&
                !estaPresente(SHELDON) && !estaPresente(AMY)) {
        cout << "Stu" << endl;
        signalCond(&stuartLiberado);
    } else if (!estaPresente(LEONARD) && !estaPresente(PENNY) &&
                !estaPresente(HOWARD) && !estaPresente(BERNADETTE) &&
                !estaPresente(SHELDON) && !estaPresente(AMY)
                && !estaPresente(STUART)) {
        cout << "K" << endl;
        signalCond(&kripkeLiberado);
    }

}

void Monitor::verificar() {
    return;
}
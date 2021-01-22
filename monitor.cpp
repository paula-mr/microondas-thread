#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <stdlib.h> 
#include <string>

using namespace std; 

void condSignal(pthread_cond_t* cond);
void waitCond(pthread_cond_t* cond, pthread_mutex_t* mutex);

void Monitor::esperar(Personagem p) {
    cout << p.name << " quer usar o forno" << endl;
    lista.insert({p.name, ordem});
    ordem++;

    if (pthread_mutex_lock(&this->mutex) != 0) {
        perror("pthread_mutex_lock error");
        exit(2);
    }

    if (p.equals(HOWARD)) {
        if (lista.size() > 1 && proximoAExecutar != HOWARD) {
            waitCond(&howardLiberado, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(BERNADETTE)) {
        if (lista.size() > 1 && proximoAExecutar != BERNADETTE) {
            waitCond(&bernadetteLiberada, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(LEONARD)) {
        if (lista.size() > 1 && proximoAExecutar != LEONARD) {
            waitCond(&leonardLiberado, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(PENNY)) {
        if (lista.size() > 1 && proximoAExecutar != PENNY) {
            waitCond(&pennyLiberada, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(SHELDON)) {
        if (lista.size() > 1 && proximoAExecutar != SHELDON) {
            waitCond(&sheldonLiberado, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(AMY)) {
        if (lista.size() > 1 && proximoAExecutar != AMY) {
            waitCond(&amyLiberada, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(STUART)) {
        if (lista.size() > 1 && proximoAExecutar != STUART) {
            waitCond(&stuartLiberado, &mutex);
            proximoAExecutar = "";
        }
    } else if (p.equals(KRIPKE)) {
        if (lista.size() > 1 && proximoAExecutar != KRIPKE) {
            waitCond(&kripkeLiberado, &mutex);
            proximoAExecutar = "";
        }
    }
  
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;
    ultimoExecutado = p.name;

    lista.erase(p.name);
    pthread_mutex_unlock(&this->mutex);

    if (deveExecutarCasal(SHELDON, AMY) && !deveExecutarCasal(LEONARD, PENNY)) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            proximoAExecutar = SHELDON;
            condSignal(&sheldonLiberado);
        } else {
            proximoAExecutar = AMY;
            condSignal(&amyLiberada);
        }
    } else if (deveExecutarCasal(LEONARD, PENNY) && !deveExecutarCasal(HOWARD, BERNADETTE)) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            proximoAExecutar = LEONARD;
            condSignal(&leonardLiberado);
        } else {
            proximoAExecutar = PENNY;
            condSignal(&pennyLiberada);
        }
    } else if (deveExecutarCasal(HOWARD, BERNADETTE) && !deveExecutarCasal(SHELDON, AMY)) {
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            proximoAExecutar = HOWARD;
            condSignal(&howardLiberado);
        } else {
            proximoAExecutar = BERNADETTE;
            condSignal(&bernadetteLiberada);
        }
    } else if ((estaPresente(SHELDON) || estaPresente(AMY)) 
        && (!estaPresente(LEONARD) && !estaPresente(PENNY))) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            proximoAExecutar = SHELDON;
            condSignal(&sheldonLiberado);
        } else {
            proximoAExecutar = AMY;
            condSignal(&amyLiberada);
        }
    } else if ((estaPresente(HOWARD) || estaPresente(BERNADETTE)) 
        && (!estaPresente(SHELDON) && !estaPresente(AMY))) {
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            proximoAExecutar = HOWARD;
            condSignal(&howardLiberado);
        } else {
            proximoAExecutar = BERNADETTE;
            condSignal(&bernadetteLiberada);
        }
    } else if ((estaPresente(LEONARD) || estaPresente(PENNY)) 
        && (!estaPresente(HOWARD) && !estaPresente(BERNADETTE))) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            proximoAExecutar = LEONARD;
            condSignal(&leonardLiberado);
        } else {
            proximoAExecutar = PENNY;
            condSignal(&pennyLiberada);
        }
    } else if (!estaPresente(LEONARD) && !estaPresente(PENNY) &&
                !estaPresente(HOWARD) && !estaPresente(BERNADETTE) &&
                !estaPresente(SHELDON) && !estaPresente(AMY)) {
        if (estaPresente(STUART)) {
            proximoAExecutar = STUART;
            condSignal(&stuartLiberado);
        } else {
            proximoAExecutar = KRIPKE;
            condSignal(&kripkeLiberado);
        }
    }

}

void Monitor::verificar() {
    return;
}


bool Monitor::estaPresente(string personagem) {
    return lista.find(personagem) != lista.end();
}

string Monitor::encontrarPrimeiro(string p1, string p2) {
    std::map<string, int>::iterator primeiro = lista.find(p1);
    std::map<string, int>::iterator segundo = lista.find(p2);

    if (primeiro == lista.end()) return p2;
    if (segundo == lista.end()) return p1;

    if (primeiro->second < segundo->second) 
        return p1;
    else
        return p2;
}

bool Monitor::casalCompleto(string p1, string p2) {
    return lista.find(p1) != lista.end() && lista.find(p2) != lista.end();
}


bool Monitor::deveExecutarCasal(const string c1, const string c2) {
    bool umFilaOutroAcabouDeSerExecutado = ((estaPresente(c1) && ultimoExecutado == c2) ||
        (estaPresente(c2) && ultimoExecutado == c1));
    bool ambosNaFila = casalCompleto(c1, c2);

    return ambosNaFila || umFilaOutroAcabouDeSerExecutado;
}

void condSignal(pthread_cond_t* cond) {
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
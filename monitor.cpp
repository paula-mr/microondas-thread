#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <stdlib.h> 
#include <string>

using namespace std; 

void waitCond(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    if (pthread_cond_wait(cond, mutex) != 0) {
        perror("pthread_cond_wait() error");
        exit(2);
    }
}

void condSignal(pthread_cond_t* cond) {
    if (pthread_cond_signal(cond) != 0) {
        perror("pthread_cond_signal() error");
        exit(2);
    }
}

void Monitor::esperar(Personagem p) {
    cout << p.nome << " quer usar o forno" << endl;
    lista.insert({p.nome, ordem++});

    if (pthread_mutex_lock(&mutex) != 0) {
        perror("pthread_mutex_lock error");
        exit(2);
    }

    if (p.deveEsperar(proximoAExecutar, lista.size())) esperarPorVez(p.nome);
}

void Monitor::liberar(Personagem p) {
    cout << p.nome << " vai comer" << endl;
    ultimoExecutado = p.nome;

    lista.erase(p.nome);
    pthread_mutex_unlock(&this->mutex);

    proximoAExecutar = definirProximoAExecutar();

    if (proximoAExecutar != "") {
        liberarPersonagem(proximoAExecutar);
    }

}

void Monitor::verificar() {

}

string Monitor::definirProximoAExecutar() {
    if (deveExecutarCasal(SHELDON, AMY) && !deveExecutarCasal(LEONARD, PENNY)) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            return SHELDON;
        } else {
            return AMY;
        }
    } else if (deveExecutarCasal(LEONARD, PENNY) && !deveExecutarCasal(HOWARD, BERNADETTE)) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            return LEONARD;
        } else {
            return PENNY;
        }
    } else if (deveExecutarCasal(HOWARD, BERNADETTE) && !deveExecutarCasal(SHELDON, AMY)) {
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            return HOWARD;
        } else {
            return BERNADETTE;
        }
    } else if ((estaPresente(SHELDON) || estaPresente(AMY)) 
        && (!estaPresente(LEONARD) && !estaPresente(PENNY))) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            return SHELDON;
        } else {
            return AMY;
        }
    } else if ((estaPresente(HOWARD) || estaPresente(BERNADETTE)) 
        && (!estaPresente(SHELDON) && !estaPresente(AMY))) {
        if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
            return HOWARD;
        } else {
            return BERNADETTE;
        }
    } else if ((estaPresente(LEONARD) || estaPresente(PENNY)) 
        && (!estaPresente(HOWARD) && !estaPresente(BERNADETTE))) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            return LEONARD;
        } else {
            return PENNY;
        }
    } else if (!estaPresente(LEONARD) && !estaPresente(PENNY) &&
                !estaPresente(HOWARD) && !estaPresente(BERNADETTE) &&
                !estaPresente(SHELDON) && !estaPresente(AMY) &&
                !estaPresente(STUART) && estaPresente(KRIPKE)) {
        return KRIPKE;
    } else if (!estaPresente(LEONARD) && !estaPresente(PENNY) &&
                !estaPresente(HOWARD) && !estaPresente(BERNADETTE) &&
                !estaPresente(SHELDON) && !estaPresente(AMY) &&
                estaPresente(STUART)) {
        return STUART;
    } else {
        cout << "DEADLOCK" << endl;
        return "";
    }

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

void Monitor::liberarPersonagem(string nome) {
    if (nome == HOWARD) {
        condSignal(&howardLiberado);
    } else if (nome == BERNADETTE) {
        condSignal(&bernadetteLiberada);
    } else if (nome == LEONARD) {
        condSignal(&leonardLiberado);
    } else if (nome == PENNY) {
        condSignal(&pennyLiberada);
    } else if (nome == SHELDON) {
        condSignal(&sheldonLiberado);
    } else if (nome == AMY) {
        condSignal(&amyLiberada);
    } else if (nome == STUART) {
        condSignal(&stuartLiberado);
    } else if (nome == KRIPKE) {
        condSignal(&kripkeLiberado);
    }
}

void Monitor::esperarPorVez(string nome) {
    if (nome == HOWARD) {
        waitCond(&howardLiberado, &mutex);
    } else if (nome == BERNADETTE) {
        waitCond(&bernadetteLiberada, &mutex);
    } else if (nome == LEONARD) {
        waitCond(&leonardLiberado, &mutex);
    } else if (nome == PENNY) {
        waitCond(&pennyLiberada, &mutex);
    } else if (nome == SHELDON) {
        waitCond(&sheldonLiberado, &mutex);
    } else if (nome == AMY) {
        waitCond(&amyLiberada, &mutex);
    } else if (nome == STUART) {
        waitCond(&stuartLiberado, &mutex);
    } else if (nome == KRIPKE) {
        waitCond(&kripkeLiberado, &mutex);
    }
}
#include "monitor.h"
#include "constants.h"

#include <iostream> 
#include <stdlib.h> 
#include <string>
#include <random>
#include <iterator>

using namespace std; 

bool Monitor::hasDeadLock() {
    bool hasDeadLockCasais = casalCompleto(LEONARD, PENNY) 
                        && casalCompleto(HOWARD, BERNADETTE)
                        && casalCompleto(SHELDON, AMY);

    bool hasDeadLockIndividual = ((estaPresente(LEONARD) || estaPresente(PENNY)) && !casalCompleto(LEONARD, PENNY)
                                && (estaPresente(HOWARD) || estaPresente(BERNADETTE)) && !casalCompleto(HOWARD, BERNADETTE)
                                && (estaPresente(SHELDON) || estaPresente(AMY)) && !casalCompleto(SHELDON, AMY));

    return hasDeadLockCasais || hasDeadLockIndividual;
}

void Monitor::esperar(Personagem p) {
    cout << p.nome << " quer usar o forno" << endl;

    mutexLock(&mutexLista);
    lista.insert({p.nome, ordem++});
    mutexUnlock(&mutexLista);

    mutexLock(&mutex);
    if (p.deveEsperar(proximoAExecutar, lista.size())) {
        esperarPorVez(p.nome);
    } else {
        proximoAExecutar = p.nome;
    }
}

void Monitor::liberar(Personagem p) {
    cout << p.nome << " vai comer" << endl;
    ultimoExecutado = p.nome;

    mutexLock(&mutexLista);

    lista.erase(p.nome);
    proximoAExecutar = definirProximoAExecutar();
    
    mutexUnlock(&mutexLista);

    mutexUnlock(&mutex);
    if (proximoAExecutar != "") {
        liberarPersonagem(proximoAExecutar);
    }

}

void Monitor::verificar() {
    random_device rd; 
    mt19937 gen(rd()); 

    if (!hasDeadLock() || lista.size() == 0 || (hasDeadLock() && proximoAExecutar != "")) return;

    mutexLock(&mutexLista);
    
    uniform_int_distribution<> distr(0, lista.size() - 1);
    auto it = lista.begin();
    advance(it, distr(gen));
    string p = it->first;
    cout << "Raj detectou um deadlock, liberando " << p << endl;
    
    proximoAExecutar = p;

    mutexUnlock(&mutexLista);

    liberarPersonagem(p);    
}

string Monitor::definirProximoAExecutar() {
    if (deveExecutarCasal(SHELDON, AMY) && 
    (!deveExecutarCasal(LEONARD, PENNY) || deveExecutarCasalMesmoComCasalMaisPrioritarioNaFila(SHELDON, AMY))) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            return SHELDON;
        } else {
            return AMY;
        }
    } else if (deveExecutarCasal(LEONARD, PENNY) && 
    (!deveExecutarCasal(HOWARD, BERNADETTE) || deveExecutarCasalMesmoComCasalMaisPrioritarioNaFila(LEONARD, PENNY))) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            return LEONARD;
        } else {
            return PENNY;
        }
    } else if (deveExecutarCasal(HOWARD, BERNADETTE) && 
    (!deveExecutarCasal(SHELDON, AMY) || deveExecutarCasalMesmoComCasalMaisPrioritarioNaFila(HOWARD, BERNADETTE))) {
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

bool Monitor::deveExecutarCasalMesmoComCasalMaisPrioritarioNaFila(const string c1, const string c2) {
    bool umFilaOutroAcabouDeSerExecutado = ((estaPresente(c1) && ultimoExecutado == c2) ||
        (estaPresente(c2) && ultimoExecutado == c1));

    bool outrosDoisCasaisEstaoNaFila = false;
    if (c1 == HOWARD || c1 == BERNADETTE) {
        outrosDoisCasaisEstaoNaFila = casalCompleto(SHELDON, AMY) && casalCompleto(LEONARD, PENNY);
    } else if (c1 == LEONARD || c1 == PENNY) {
        outrosDoisCasaisEstaoNaFila = casalCompleto(SHELDON, AMY) && casalCompleto(HOWARD, BERNADETTE);
    } else if (c1 == SHELDON || c1 == AMY) {
        outrosDoisCasaisEstaoNaFila = casalCompleto(HOWARD, BERNADETTE) && casalCompleto(LEONARD, PENNY);
    }

    return umFilaOutroAcabouDeSerExecutado && outrosDoisCasaisEstaoNaFila;
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

void Monitor::initCond(pthread_cond_t* cond) {
    if (pthread_cond_init(cond, NULL) != 0) {
        perror("pthread_cond_init() error");
        exit(2);
    }
}

void Monitor::destroyCond(pthread_cond_t* cond) {
    if (pthread_cond_destroy(cond) != 0) {
        perror("pthread_cond_destroy() error");
        exit(2);
    }
}

void Monitor::waitCond(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    if (pthread_cond_wait(cond, mutex) != 0) {
        perror("pthread_cond_wait() error");
        exit(2);
    }
}

void Monitor::condSignal(pthread_cond_t* cond) {
    if (pthread_cond_signal(cond) != 0) {
        perror("pthread_cond_signal() error");
        exit(2);
    }
}

void Monitor::initMutex(pthread_mutex_t* mutex) {
    if (pthread_mutex_init(mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(2);
    }
}

void Monitor::destroyMutex(pthread_mutex_t* mutex) {
    if (pthread_mutex_destroy(mutex) != 0) {
        perror("pthread_mutex_destroy() error");
        exit(2);
    }
}

void Monitor::mutexLock(pthread_mutex_t* mutex) {
    if (pthread_mutex_lock(mutex) != 0) {
        perror("pthread_mutex_lock error");
        exit(2);
    }
}

void Monitor::mutexUnlock(pthread_mutex_t* mutex) {
    if (pthread_mutex_unlock(mutex) != 0) {
        perror("pthread_mutex_unlock error");
        exit(2);
    }
}
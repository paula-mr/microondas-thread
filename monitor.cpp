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
    std::map<string, int>::iterator primeiro = lista.find(p1);
    std::map<string, int>::iterator segundo = lista.find(p2);

    if (primeiro == lista.end()) return p2;
    if (segundo == lista.end()) return p1;

    if (primeiro->second < segundo->second) 
        return p1;
    else
        return p2;
}

bool casalCompleto(string casal1, string casal2) {
    return lista.find(casal1) != lista.end() && lista.find(casal2) != lista.end();
}

bool nenhumMembroDoCasal(const string casal1, const string casal2) {
    return lista.find(casal1) == lista.end() && lista.find(casal2) == lista.end();
}

bool deveExecutarCasal(const string c1, const string c2, string ultimoExecutado) {
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

void Monitor::esperarPorCasalHowardEBernadette() {
    if (!deveExecutarCasal(HOWARD, BERNADETTE, ultimoExecutado)) 
        return;

    if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
        waitCond(&howardFoiComer, &mutex);
        if (estaPresente(BERNADETTE) && ultimoExecutado == HOWARD) {
            waitCond(&bernadetteFoiComer, &mutex);
        }
    } else {
        cout << "esperando por b" << endl;
        waitCond(&bernadetteFoiComer, &mutex);
        cout << "b liberou" << endl;
        if (estaPresente(HOWARD) && ultimoExecutado == BERNADETTE) {
            cout << "esperando por h" << endl;
            waitCond(&howardFoiComer, &mutex);
        }
    }

}

void Monitor::esperarPorCasalSheldonEAmy() {
    if (deveExecutarCasal(SHELDON, AMY, ultimoExecutado)) {
        if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
            waitCond(&sheldonFoiComer, &mutex);
            if (estaPresente(AMY)) 
                waitCond(&amyFoiComer, &mutex);
        } else {
            waitCond(&amyFoiComer, &mutex);
            if (estaPresente(SHELDON)) 
                waitCond(&sheldonFoiComer, &mutex);
        }
    }
}

void Monitor::esperarPorCasalLeonardEPenny() {
    if (deveExecutarCasal(LEONARD, PENNY, ultimoExecutado)) {
        if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
            waitCond(&leonardFoiComer, &mutex);
            if (estaPresente(PENNY)) 
                waitCond(&pennyFoiComer, &mutex);
        } else {
            waitCond(&pennyFoiComer, &mutex);
            if (estaPresente(LEONARD)) 
                waitCond(&leonardFoiComer, &mutex);
        }
    }
}

void Monitor::esperarPorHowardEBernadetteSeparadamente() {
    if (estaPresente(HOWARD)) {
        waitCond(&howardFoiComer, &mutex);
        if (estaPresente(BERNADETTE)) {
            waitCond(&bernadetteFoiComer, &mutex);
        }
    } else if (estaPresente(BERNADETTE)) {
        waitCond(&bernadetteFoiComer, &mutex);
        if (estaPresente(HOWARD)) {
            waitCond(&howardFoiComer, &mutex);
        }
    }
}

void Monitor::esperarPorSheldonEAmySeparadamente() {
    if (estaPresente(AMY)) {
        waitCond(&amyFoiComer, &mutex);
        if (estaPresente(SHELDON)) {
            waitCond(&sheldonFoiComer, &mutex);
        }
    } else if (estaPresente(SHELDON)) {
        waitCond(&sheldonFoiComer, &mutex);
        if (estaPresente(AMY)) {
            waitCond(&amyFoiComer, &mutex);
        }
    }
}

void Monitor::esperarPorLeonardEPennySeparadamente() {
    if (estaPresente(LEONARD)) {
        waitCond(&leonardFoiComer, &mutex);
        if (estaPresente(PENNY)) {
            waitCond(&pennyFoiComer, &mutex);
        }
    } else if (estaPresente(PENNY)) {
        waitCond(&pennyFoiComer, &mutex);
        if (estaPresente(LEONARD)) {
            waitCond(&leonardFoiComer, &mutex);
        }
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

    // cout << p.name << " has lock" << endl;

    if (p.equals(LEONARD)) {
        if (deveExecutarCasal(LEONARD, PENNY, ultimoExecutado)) {
            if (encontrarPrimeiro(LEONARD, PENNY) == PENNY) {
                waitCond(&leonardOuPennyDeixouOOutroComer, &mutex);
            }
            esperarPorCasalHowardEBernadette();
        } else {
            esperarPorHowardEBernadetteSeparadamente();
            esperarPorCasalSheldonEAmy();
        }
    } else if (p.equals(PENNY)) {
        if (deveExecutarCasal(LEONARD, PENNY, ultimoExecutado)) {
            if (encontrarPrimeiro(LEONARD, PENNY) == LEONARD) {
                waitCond(&leonardOuPennyDeixouOOutroComer, &mutex);
            }
            esperarPorCasalHowardEBernadette();
        } else {
            esperarPorHowardEBernadetteSeparadamente();
            esperarPorCasalSheldonEAmy();
        }
    } else if (p.equals(HOWARD)) {
        if (deveExecutarCasal(HOWARD, BERNADETTE, ultimoExecutado)) {
            if (encontrarPrimeiro(HOWARD, BERNADETTE) == BERNADETTE) {
                waitCond(&howardOuBernadetteDeixouOOutroComer, &mutex);
            }
            esperarPorCasalSheldonEAmy();
        } else {
            esperarPorSheldonEAmySeparadamente();
            esperarPorCasalLeonardEPenny();
        }
    } else if (p.equals(BERNADETTE)) {
        if (deveExecutarCasal(HOWARD, BERNADETTE, ultimoExecutado)) {
            if (encontrarPrimeiro(HOWARD, BERNADETTE) == HOWARD) {
                waitCond(&howardOuBernadetteDeixouOOutroComer, &mutex);
            }
            esperarPorCasalSheldonEAmy();
        } else {
            esperarPorSheldonEAmySeparadamente();
            esperarPorCasalLeonardEPenny();
        }
    } else if (p.equals(SHELDON)) {
        if (deveExecutarCasal(SHELDON, AMY, ultimoExecutado)) {
            if (encontrarPrimeiro(SHELDON, AMY) == AMY) {
                waitCond(&sheldonOuAmyDeixouOOutroComer, &mutex);
            }
            esperarPorCasalLeonardEPenny();
        } else {
            esperarPorLeonardEPennySeparadamente();
            esperarPorCasalHowardEBernadette();
        }
    } else if (p.equals(AMY)) {
        if (deveExecutarCasal(SHELDON, AMY, ultimoExecutado)) {
            if (encontrarPrimeiro(SHELDON, AMY) == SHELDON) {
                waitCond(&sheldonOuAmyDeixouOOutroComer, &mutex);
            }
            esperarPorCasalLeonardEPenny();
        } else {
            esperarPorLeonardEPennySeparadamente();
            esperarPorCasalHowardEBernadette();
        }
    } else if (p.equals(STUART)) {
        esperarPorCasalHowardEBernadette();
        esperarPorCasalLeonardEPenny();
        esperarPorCasalSheldonEAmy();
        esperarPorHowardEBernadetteSeparadamente();
        esperarPorLeonardEPennySeparadamente();
        esperarPorSheldonEAmySeparadamente();
    } else if (p.equals(KRIPKE)) {
        esperarPorCasalHowardEBernadette();
        esperarPorCasalLeonardEPenny();
        esperarPorCasalSheldonEAmy();
        esperarPorHowardEBernadetteSeparadamente();
        esperarPorLeonardEPennySeparadamente();
        esperarPorSheldonEAmySeparadamente();
        waitCond(&stuartFoiComer, &mutex);
    }
  
}

void Monitor::liberar(Personagem p) {
    cout << p.name << " vai comer" << endl;
    ultimoExecutado = p.name;

    lista.erase(p.name);
    pthread_mutex_unlock(&this->mutex);

    if (p.equals(SHELDON) || p.equals(AMY)) {
        if (deveExecutarCasal(SHELDON, AMY, ultimoExecutado)) {
            condSignal(&sheldonOuAmyDeixouOOutroComer);
        } else {
            pthread_cond_t* quemFoiComer = p.equals(SHELDON) ? &sheldonFoiComer : &amyFoiComer;
            condSignal(quemFoiComer);
        }
    } else if (p.equals(LEONARD) || p.equals(PENNY)) {
        if (deveExecutarCasal(LEONARD, PENNY, ultimoExecutado)) {
            condSignal(&leonardOuPennyDeixouOOutroComer);
        } else {
            pthread_cond_t* quemFoiComer = p.equals(LEONARD) ? &leonardFoiComer : &pennyFoiComer;
            condSignal(quemFoiComer);
        }
    } else if (p.equals(HOWARD) || p.equals(BERNADETTE)) {
        if (deveExecutarCasal(HOWARD, BERNADETTE, ultimoExecutado)) {
            condSignal(&howardOuBernadetteDeixouOOutroComer);
        } else {
            pthread_cond_t* quemFoiComer = p.equals(HOWARD) ? &howardFoiComer : &bernadetteFoiComer;
            condSignal(quemFoiComer);
        }
    } else if (p.equals(STUART)) {
        condSignal(&stuartFoiComer);
    }
}

void Monitor::verificar() {
    return;
}
#include "personagem.h"
#include "constants.h"

#include <unistd.h>
#include <iostream> 
#include <stdlib.h> 
#include <random>
#include <string.h>
using namespace std; 

random_device rd; 
mt19937 gen(rd()); 
uniform_int_distribution<> distr(3, 6);

bool Personagem::deveEsperar(string proximoAExecutar, int tamanhoFila) {
    return tamanhoFila > 1 && proximoAExecutar != nome;
}

void Personagem::esquentarAlgo() {
    cout << this->nome << " começa a esquentar algo" << endl;
    sleep(1);
}

void Personagem::comer() {
    sleep(distr(gen));
}

void Personagem::voltarATrabalhar() {
    cout << this->nome << " voltou para o trabalho" << endl;
    sleep(distr(gen));
}

bool Personagem::equals(string nome) {
    return this->nome == nome;
}

Personagem encontrarPersonagemPorNome(const char* nome) {
    for (Personagem p : personagens) {
        if (p.equals(nome)) return p;
    }

    return personagens[0];
}
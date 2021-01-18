#include "personagem.h"

#include <unistd.h>
#include <iostream> 
#include <stdlib.h> 
#include <random>
#include <string.h>

using namespace std; 

random_device rd; 
mt19937 gen(rd()); 
uniform_int_distribution<> distr(3, 6);

void Personagem::esquentarAlgo() {
    cout << this->name << " começa a esquentar algo" << endl;
    sleep(1);
}

void Personagem::comer() {
    sleep(distr(gen));
}

void Personagem::voltarATrabalhar() {
    cout << this->name << " voltou para o trabalho" << endl;
    sleep(distr(gen));
}

bool Personagem::equals(char* name) {
    return strcmp(this->name, name) == 0;
}

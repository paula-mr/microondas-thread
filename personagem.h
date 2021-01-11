#pragma once

#include <string>
#include <pthread.h>

using namespace std; 

class Personagem {
    public:
        char* name;
        pthread_t id;

        void esquentarAlgo();
        void comer();
        void voltarATrabalhar();

        Personagem(char* name) {
            this->name = name;
        }
};
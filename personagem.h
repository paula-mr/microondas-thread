#pragma once

#include <string>
#include <pthread.h>
#include "constants.h"

using namespace std; 

class Personagem {
    public:
        char* name;
        pthread_t id;

        void esquentarAlgo();
        void comer();
        void voltarATrabalhar();
        bool equals(const char* name);

        Personagem(char* name) {
            this->name = name;
        }
};
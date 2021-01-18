#pragma once

#include <string>
#include <pthread.h>
#include "constants.h"

using namespace std; 

class Personagem {
    public:
        string name;
        pthread_t id;

        void esquentarAlgo();
        void comer();
        void voltarATrabalhar();
        bool equals(string name);

        Personagem(string name) {
            this->name = name;
        }
};
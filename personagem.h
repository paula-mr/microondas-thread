#pragma once

#include "constants.h"

#include <string>
#include <pthread.h>

using namespace std; 

class Personagem {
    public:
        string nome;

        bool deveEsperar(string proximoAExecutar, int tamanhoFila);
        void esquentarAlgo();
        void comer();
        void voltarATrabalhar();

        bool equals(string nome);

        Personagem(string nome) {
            this->nome = nome;
        }
};

const Personagem personagens[] = {
                                    Personagem((char*) "Kripke"), 
                                    Personagem((char*) "Stuart"),
                                    Personagem((char*) "Sheldon"),
                                    Personagem((char*) "Howard"),
                                    Personagem((char*) "Penny"), 
                                    Personagem((char*) "Leonard"), 
                                    Personagem((char*) "Amy"), 
                                    Personagem((char*) "Bernadette")
                                };

Personagem encontrarPersonagemPorNome(const char* nome); 
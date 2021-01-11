#include "monitor.h"
#include "personagem.h"

#include <iostream> 
#include <pthread.h>
#include <string>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>

using namespace std; 

void *startCharacter(void *character);
pthread_t startThread(char* character);
void waitForThreadToFinish(pthread_t id);


Monitor forno;

int quantidadeUsoForno;

Personagem personagens[] = {Personagem((char*) "Penny"), Personagem((char*) "Leonard"), 
                            Personagem((char*) "Amy"), Personagem((char*) "Sheldon"),
                            Personagem((char*) "Howard"), Personagem((char*) "Bernadette"),
                            Personagem((char*) "Kripke"), Personagem((char*) "Stuart")};

Personagem encontrarPersonagemPorNome(char* nome) {
    for (Personagem p : personagens) {
        if (p.name == nome) return p;
    }
    return NULL;
}

pthread_t startThread(char* character) {
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, startCharacter, (void *) character) < 0) {
        perror("Não foi possível iniciar a thread");
    }
    
    return thread_id;
}

void waitForThreadToFinish(pthread_t id) {
     if (pthread_join(id, NULL) < 0) {
        perror("Erro ao finalizar thread");
    }
}

void *startCharacter(void *character) {
    Personagem p = encontrarPersonagemPorNome((char *) character);

    for (int i=0; i < quantidadeUsoForno; i++) {
        forno.esperar(p);
        p.esquentarAlgo();
        forno.liberar(p);
        p.comer();
        p.voltarATrabalhar();
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Necessário passar a quantidade de vezes que um personagem deseja comer." << endl;
        return 1;
    }

    quantidadeUsoForno = atoi(argv[1]);

    for (Personagem p: personagens) {
        p.id = startThread(p.name);
        sleep(2);
    }

    for (Personagem p: personagens) {
        waitForThreadToFinish(p.id);
    }


    return 0;
}
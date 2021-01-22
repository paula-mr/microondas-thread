#include "monitor.h"
#include "personagem.h"

#include <iostream> 
#include <pthread.h>
#include <string>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <vector>

using namespace std; 

void *startCharacter(void *character);
pthread_t startThread(char* character);
void waitForThreadToFinish(pthread_t id);


Monitor forno;

int quantidadeUsoForno;

Personagem personagens[] = {Personagem((char*) "Sheldon"),
                            Personagem((char*) "Howard"),
                            Personagem((char*) "Penny"), 
                            Personagem((char*) "Leonard"), 
                            Personagem((char*) "Amy"), 
                            Personagem((char*) "Bernadette"),
                            Personagem((char*) "Kripke"), 
                            Personagem((char*) "Stuart")};


Personagem encontrarPersonagemPorNome(char* name) {
    for (Personagem p : personagens) {
        if (p.equals(name)) return p;
    }

    return personagens[0];
}

pthread_t startThread(const char* character) {
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, startCharacter, (void *) character) < 0) {
        perror("Não foi possível iniciar a thread");
    }
    
    return thread_id;
}

void waitForThreadToFinish(pthread_t id) {
    int ret = pthread_join(id, NULL);
    if (ret != 0) {
        perror("Erro ao finalizar thread");
        cout << "\nErro: " << ret << '\n';
        exit(2);
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

    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Necessário passar a quantidade de vezes que um personagem deseja comer." << endl;
        return 1;
    }

    quantidadeUsoForno = atoi(argv[1]);
    vector<pthread_t> t_ids;
    for (Personagem p: personagens) {
        pthread_t res = startThread(p.name.c_str());
        t_ids.push_back(res);
        p.id = res; // por que isso n funciona ???
        sleep(1);
    }

    for (const auto t_id: t_ids) {
        waitForThreadToFinish(t_id);
    }

    return 0;
}
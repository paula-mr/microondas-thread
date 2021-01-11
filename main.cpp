#include "monitor.h"
#include "personagem.h"

#include <iostream> 
#include <pthread.h>
#include <string>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 

using namespace std; 

void *startCharacter(void *character);
pthread_t startThread(char* character);

Monitor forno;

pthread_t startThread(char* character) {
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, startCharacter, (void *) character) < 0) {
        perror("Não foi possível iniciar a thread");
    }
    cout << "Thread de conexões aberta com sucesso! Iniciando personagem " << character << endl;
    return thread_id;
}

void *startCharacter(void *character) {
    Personagem p;
    p.name = (char*) character;

    while (true) {
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

    int quantidade = atoi(argv[1]);
    pthread_t penny = startThread("Penny");
    return 0;
}
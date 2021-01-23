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

void *iniciarPersonagem(void *character);
pthread_t iniciarThread(char* character);
void esperarThreadTerminar(pthread_t id);


Monitor forno;
bool should_run_raj = true;

int quantidadeUsoForno;

pthread_t iniciarThread(const char* personagem) {
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, iniciarPersonagem, (void *) personagem) < 0) {
        perror("Não foi possível iniciar a thread");
    }
    
    return thread_id;
}

void esperarThreadTerminar(pthread_t id) {
    int ret = pthread_join(id, NULL);
    if (ret != 0) {
        perror("Erro ao finalizar thread");
        cout << "\nErro: " << ret << '\n';
        exit(2);
    }
}

void *iniciarPersonagem(void *nome) {
    Personagem p = encontrarPersonagemPorNome((char *) nome);

    for (int i=0; i < quantidadeUsoForno; i++) {
        forno.esperar(p);
        p.esquentarAlgo();
        forno.liberar(p);
        p.comer();
        p.voltarATrabalhar();
    }

    return NULL;
}

void *run_raj(void* flag) {
    bool should_run_raj = ((bool*) flag);
    cout << "should_run_raj=" << (should_run_raj ? "true" : "false") << '\n';
    while (should_run_raj) {
        sleep(5);
        forno.verificar();
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Necessário passar a quantidade de vezes que um personagem deseja comer." << endl;
        return 1;
    }

    quantidadeUsoForno = atoi(argv[1]);
    
    pthread_t raj;

    if (pthread_create(&raj, NULL, run_raj, &should_run_raj) < 0) {
        perror("Não foi possível iniciar a thread");
    }
    
    vector<pthread_t> t_ids;
    for (Personagem p: personagens) {
        pthread_t res = iniciarThread(p.nome.c_str());
        t_ids.push_back(res);
        sleep(1);
    }

    for (const auto t_id: t_ids) {
        esperarThreadTerminar(t_id);
    }

    should_run_raj = false;

    return 0;
}
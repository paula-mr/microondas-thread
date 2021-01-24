#pragma once

#include "personagem.h"

#include <pthread.h>
#include <map>

class Monitor {
    private:
        void initCond(pthread_cond_t* cond) {
            if (pthread_cond_init(cond, NULL) != 0) {
                perror("pthread_cond_init() error");
                exit(2);
            }
        }

        void destroyCond(pthread_cond_t* cond) {
            if (pthread_cond_destroy(cond) != 0) {
                perror("pthread_cond_destroy() error");
                exit(2);
            }
        }

    public:
        int ordem;
        string ultimoExecutado;
        string proximoAExecutar;
        map<string, int> lista;

        pthread_mutex_t mutex;
        pthread_mutex_t mutexLista;

        pthread_cond_t sheldonLiberado;
        pthread_cond_t amyLiberada;
        pthread_cond_t howardLiberado;
        pthread_cond_t bernadetteLiberada;
        pthread_cond_t leonardLiberado;
        pthread_cond_t pennyLiberada;
        pthread_cond_t stuartLiberado;
        pthread_cond_t kripkeLiberado;

        Monitor() {
            if (pthread_mutex_init(&mutex, NULL) != 0) {
                perror("pthread_mutex_init");
                exit(2);
            }
            if (pthread_mutex_init(&mutexLista, NULL) != 0) {
                perror("pthread_mutexLista_init");
                exit(2);
            }
            initCond(&sheldonLiberado);
            initCond(&amyLiberada);
            initCond(&howardLiberado);
            initCond(&bernadetteLiberada);    
            initCond(&leonardLiberado);
            initCond(&pennyLiberada);
            initCond(&stuartLiberado);  
            initCond(&kripkeLiberado);        

            ordem = 0;                  
        }

        ~Monitor() {
            if (pthread_mutex_destroy(&mutex) != 0) {
                perror("pthread_mutex_destroy() error");
                exit(2);
            }
            if (pthread_mutex_destroy(&mutexLista) != 0) {
                perror("pthread_mutex_destroy() error");
                exit(2);
            }
            destroyCond(&sheldonLiberado);
            destroyCond(&amyLiberada);
            destroyCond(&howardLiberado);
            destroyCond(&bernadetteLiberada);    
            destroyCond(&leonardLiberado);
            destroyCond(&pennyLiberada);
            destroyCond(&stuartLiberado);  
            destroyCond(&kripkeLiberado);       
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();

        bool estaPresente(string personagem);
        string encontrarPrimeiro(string p1, string p2);
        bool deveExecutarCasal(const string c1, const string c2);
        bool casalCompleto(string p1, string p2);
        void liberarPersonagem(string nome);
        void esperarPorVez(string nome);
        string definirProximoAExecutar();
        bool hasDeadLock();
};
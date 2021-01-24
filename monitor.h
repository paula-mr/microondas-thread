#pragma once

#include "personagem.h"

#include <pthread.h>
#include <map>

class Monitor {
    private:
        bool estaPresente(string personagem);
        string encontrarPrimeiro(string p1, string p2);
        bool deveExecutarCasal(const string c1, const string c2);
        bool deveExecutarCasalMesmoComCasalMaisPrioritarioNaFila(const string c1, const string c2);
        bool casalCompleto(string p1, string p2);
        void liberarPersonagem(string nome);
        void esperarPorVez(string nome);
        string definirProximoAExecutar();
        bool hasDeadLock();
        void initCond(pthread_cond_t* cond);
        void destroyCond(pthread_cond_t* cond);
        void waitCond(pthread_cond_t* cond, pthread_mutex_t* mutex);
        void condSignal(pthread_cond_t* cond);
        void initMutex(pthread_mutex_t* mutex);
        void destroyMutex(pthread_mutex_t* mutex);
        void mutexLock(pthread_mutex_t* mutex);
        void mutexUnlock(pthread_mutex_t* mutex);

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
            initMutex(&mutex);
            initMutex(&mutexLista);
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
            destroyMutex(&mutex);
            destroyMutex(&mutexLista);
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
};
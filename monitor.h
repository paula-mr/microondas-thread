#pragma once

#include "personagem.h"

#include <pthread.h>

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
        pthread_mutex_t mutex;
        pthread_attr_t attr;

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
            if (pthread_attr_init(&attr) != 0) {
                perror("pthread_attr_init()");
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
            if (pthread_attr_destroy(&attr) != 0) {
                perror("pthread_attr_destroy() error");
                exit(2);
            }
            if (pthread_mutex_destroy(&mutex) != 0) {
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
};
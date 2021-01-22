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
        pthread_mutex_t mutex;
        pthread_attr_t attr;

        pthread_cond_t sheldonFoiComer;
        pthread_cond_t amyFoiComer;
        pthread_cond_t howardFoiComer;
        pthread_cond_t bernadetteFoiComer;
        pthread_cond_t leonardFoiComer;
        pthread_cond_t pennyFoiComer;
        pthread_cond_t stuartFoiComer;
        pthread_cond_t kripkeFoiComer;

        Monitor() {
            if (pthread_mutex_init(&mutex, NULL) != 0) {
                perror("pthread_mutex_init");
                exit(2);
            }
            if (pthread_attr_init(&attr) != 0) {
                perror("pthread_attr_init()");
                exit(2);
            }
            initCond(&sheldonFoiComer);
            initCond(&amyFoiComer);
            initCond(&howardFoiComer);
            initCond(&bernadetteFoiComer);    
            initCond(&leonardFoiComer);
            initCond(&pennyFoiComer);
            initCond(&stuartFoiComer);  
            initCond(&kripkeFoiComer);        
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
            destroyCond(&sheldonFoiComer);
            destroyCond(&amyFoiComer);
            destroyCond(&howardFoiComer);
            destroyCond(&bernadetteFoiComer);    
            destroyCond(&leonardFoiComer);
            destroyCond(&pennyFoiComer);
            destroyCond(&stuartFoiComer);  
            destroyCond(&kripkeFoiComer);   
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();

        void esperarPorCasalHowardEBernadette();
        void esperarPorHowardEBernadetteSeparadamente();
        void esperarPorCasalSheldonEAmy();
        void esperarPorSheldonEAmySeparadamente();
        void esperarPorCasalLeonardEPenny();
        void esperarPorLeonardEPennySeparadamente();
};
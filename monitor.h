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
        pthread_mutex_t mutex;
        pthread_attr_t attr;
        pthread_cond_t sheldonAndAmyDontWantToUse;
        pthread_cond_t howardAndBernadetteDontWantToUse;
        pthread_cond_t leonardAndPennyDontWantToUse;
        pthread_cond_t stuartDoesntWantToUse;

        pthread_cond_t sheldonAndAmyAreNotTogether;
        pthread_cond_t howardAndBernadetteAreNotTogether;
        pthread_cond_t leonardAndPennyAreNotTogether;

        bool casalSheldonEAmyAtivo;
        bool casalLeonardEPennyAtivo;
        bool casalHowardEBernadetteAtivo;

        Monitor() {
            if (pthread_mutex_init(&mutex, NULL) != 0) {
                perror("pthread_mutex_init");
                exit(2);
            }
            if (pthread_attr_init(&attr) != 0) {
                perror("pthread_attr_init()");
                exit(2);
            }
            initCond(&sheldonAndAmyDontWantToUse);
            initCond(&howardAndBernadetteDontWantToUse);
            initCond(&leonardAndPennyDontWantToUse);
            initCond(&stuartDoesntWantToUse);    
            initCond(&sheldonAndAmyAreNotTogether);
            initCond(&howardAndBernadetteAreNotTogether);
            initCond(&leonardAndPennyAreNotTogether);     

            casalSheldonEAmyAtivo = false;
            casalLeonardEPennyAtivo = false;
            casalHowardEBernadetteAtivo = false;                                        
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
            destroyCond(&sheldonAndAmyDontWantToUse);
            destroyCond(&howardAndBernadetteDontWantToUse);
            destroyCond(&leonardAndPennyDontWantToUse);
            destroyCond(&stuartDoesntWantToUse);
            destroyCond(&sheldonAndAmyAreNotTogether);
            destroyCond(&howardAndBernadetteAreNotTogether);
            destroyCond(&leonardAndPennyAreNotTogether); 
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();

        void esperarPorSheldonOuAmy();
        void esperarPorHowardOuBernadette();
        void esperarPorLeonardOuPenny();

        void esperarPorSheldonEAmy();
        void esperarPorHowardEBernadette();
        void esperarPorLeonardEPenny();
        void esperarPorStuart();
};
#pragma once

#include "personagem.h"

#include <pthread.h>

class Monitor {
    public:
        pthread_mutex_t mutex;
        pthread_attr_t attr;
        pthread_cond_t sheldonAndAmyDontWantToUse;
        pthread_cond_t howardAndBernadetteDontWantToUse;
        pthread_cond_t leonardAndPennyDontWantToUse;
        pthread_cond_t stuartDoesntWantToUse;

        Monitor() {
            if (pthread_mutex_init(&mutex, NULL) != 0) {
                perror("pthread_mutex_init");
                exit(2);
            }
            if (pthread_attr_init(&attr) != 0) {
                perror("pthread_attr_init()");
                exit(2);
            }
            if (pthread_cond_init(&sheldonAndAmyDontWantToUse, NULL) != 0) {
                perror("pthread_cond_init() error");
                exit(2);
            }
            if (pthread_cond_init(&howardAndBernadetteDontWantToUse, NULL) != 0) {
                perror("pthread_cond_init() error");
                exit(2);
            }
            if (pthread_cond_init(&leonardAndPennyDontWantToUse, NULL) != 0) {
                perror("pthread_cond_init() error");
                exit(2);
            }
            if (pthread_cond_init(&stuartDoesntWantToUse, NULL) != 0) {
                perror("pthread_cond_init() error");
                exit(2);
            }                                                        
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
            if (pthread_cond_destroy(&sheldonAndAmyDontWantToUse) != 0) {
                perror("pthread_cond_destroy() error");
                exit(2);
            }
            if (pthread_cond_destroy(&howardAndBernadetteDontWantToUse) != 0) {
                perror("pthread_cond_destroy() error");
                exit(2);
            }
            if (pthread_cond_destroy(&leonardAndPennyDontWantToUse) != 0) {
                perror("pthread_cond_destroy() error");
                exit(2);
            }
            if (pthread_cond_destroy(&stuartDoesntWantToUse) != 0) {
                perror("pthread_cond_destroy() error");
                exit(2);
            }
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();

        void esperarPorSheldonEAmy();
        void esperarPorHowardEBernadette();
        void esperarPorLeonardEPenny();
        void esperarPorStuart();
};
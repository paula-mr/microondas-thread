#pragma once

#include "personagem.h"

class Monitor {
    public:
        pthread_mutex_t mutex;
        pthread_attr_t attr;
        pthread_cond_t sheldonAndAmyDontWantToUse;
        pthread_cond_t howardAndBernadetteDontWantToUse;
        pthread_cond_t leonardAndPennyDontWantToUse;
        pthread_cond_t stuartDoesntWantToUse;

        Monitor() {
            pthread_mutex_init(&mutex, NULL);
            pthread_attr_init(&attr);
            pthread_cond_init(&sheldonAndAmyDontWantToUse, NULL);
            pthread_cond_init(&howardAndBernadetteDontWantToUse, NULL);
            pthread_cond_init(&leonardAndPennyDontWantToUse, NULL);
            pthread_cond_init(&stuartDoesntWantToUse, NULL);
            /*TODO 
            if (pthread_cond_destroy(&cond) != 0) {                                       
                perror("pthread_cond_destroy() error");                                     
                exit(2);    
            }*/                                                                   
        }

        ~Monitor() {
            pthread_attr_destroy(&attr);
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&sheldonAndAmyDontWantToUse);
            pthread_cond_destroy(&howardAndBernadetteDontWantToUse);
            pthread_cond_destroy(&leonardAndPennyDontWantToUse);
            pthread_cond_destroy(&stuartDoesntWantToUse);
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();

        void esperarPorSheldonEAmy();
        void esperarPorHowardEBernadette();
        void esperarPorLeonardEPenny();
        void esperarPorStuart();
};
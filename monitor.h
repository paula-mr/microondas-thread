#pragma once

#include "personagem.h"

class Monitor {
    public:
        pthread_mutex_t mutex;
        pthread_attr_t attr;
        pthread_cond_t sheldonDoesNotWantToUse;

        Monitor() {
            pthread_mutex_init(&mutex, NULL);
            pthread_attr_init(&attr);
            pthread_cond_init(&sheldonDoesNotWantToUse, NULL);
            /*TODO 
            if (pthread_cond_destroy(&cond) != 0) {                                       
                perror("pthread_cond_destroy() error");                                     
                exit(2);    
            }*/                                                                   
        }

        ~Monitor() {
            pthread_attr_destroy(&attr);
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&sheldonDoesNotWantToUse);
        }

        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();
};
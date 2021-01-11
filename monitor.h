#pragma once

#include "personagem.h"

class Monitor {
    public:
        void esperar(Personagem p);
        void liberar(Personagem p);
        void verificar();
};
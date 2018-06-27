
#include "Ponto.h"


Ponto::Ponto() {
}

Ponto::Ponto(const Ponto& orig) {
}

Ponto::~Ponto() {
}

Ponto::Ponto(double a, double b){
    Coordenada p(a,b,0);
    addCoordenada(p);
    tipo = 0;
}

Ponto::Ponto(double a, double b,std::string n){
    Coordenada p(a,b,0);
    addCoordenada(p);
    nome = n;
}

    
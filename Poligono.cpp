
#include "Poligono.h"

Poligono::Poligono(){

}
Poligono::Poligono(int size, std::string n) {
    numeroPontos = size;
    nome = n;
    posicao = 0;
}

Poligono::Poligono(const Poligono& orig) {
}

Poligono::~Poligono() {
}

void Poligono::addP(Ponto p){
    pontos[posicao] = p;
    posicao += 1;
}
void Poligono::setP(Ponto ponto, int a){	 	  	     	  	      	     	 	    	        	 	
    if(a<numeroPontos){
        pontos[a] = ponto;
    }
}
double Poligono::getX(int pos){
    if(pos<numeroPontos){
        double x = pontos[pos].getX();
        return x;
    }
}

double Poligono::getY(int pos){
    if(pos<numeroPontos){
        double y = pontos[pos].getY();
        return y;
    }
}

int Poligono::getSize(){
    return numeroPontos;
}

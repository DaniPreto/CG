
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
int Poligono::getSize(){
    return numeroPontos;
}

void Poligono::addP(double x, double y){
    Ponto p(x,y);
    pontos[posicao] = p;
    posicao += 1;
}
void Poligono::setP(double x, double y, int a){	 	  	     	  	      	     	 	    	        	 	
    Ponto ponto(x,y);
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


//------------------------------------------------------------------------------SCN

void Poligono::setPSCN(double x, double y, int a){	 	  	     	  	      	     	 	    	        	 	
    if(a<numeroPontos){
        pontos[a].setU(x);
        pontos[a].setV(y);
    }
}
double Poligono::getU(int pos){
    if(pos<numeroPontos){
        double x = pontos[pos].getU();
        return x;
    }
}

double Poligono::getV(int pos){
    if(pos<numeroPontos){
        double y = pontos[pos].getV();
        return y;
    }
}












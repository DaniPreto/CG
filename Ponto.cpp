
#include "Ponto.h"


Ponto::Ponto() {
}

Ponto::Ponto(const Ponto& orig) {
}

Ponto::~Ponto() {
}

Ponto::Ponto(int a, int b){
    x = a;
    y = b;
    }

Ponto::Ponto(int a, int b,std::string n){
    x = a;
    y = b;
    nome = n;
    }

void Ponto::setX(int v){
    x = v;
    }

void Ponto::setY(int v){
    y = v;
    }	 	  	     	  	      	     	 	    	        	 	

int Ponto::getX(){
    return x;
    }

int Ponto::getY(){
    return y;
    }

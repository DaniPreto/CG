
#include "Ponto.h"


Ponto::Ponto() {
}

Ponto::Ponto(const Ponto& orig) {
}

Ponto::~Ponto() {
}

Ponto::Ponto(double a, double b){
    x = a;
    y = b;
    }

Ponto::Ponto(double a, double b,std::string n){
    x = a;
    y = b;
    nome = n;
    }

void Ponto::setX(double valor){
    x = valor;
    }

void Ponto::setY(double valor){
    y = valor;
    }	 	  	     	  	      	     	 	    	        	 	

double Ponto::getX(){
    return x;
    }

double Ponto::getY(){
    return y;
    }
    

//-----------------------------------------------------------------PPT


void Ponto::setU(double valor){
    u = valor;
    }

void Ponto::setV(double valor){
    v = valor;
    }	 	  	     	  	      	     	 	    	        	 	

double Ponto::getU(){
    return u;
    }

double Ponto::getV(){
    return v;
    }
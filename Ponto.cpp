
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

void Ponto::setX(double v){
    x = v;
    }

void Ponto::setY(double v){
    y = v;
    }	 	  	     	  	      	     	 	    	        	 	

double Ponto::getX(){
    return x;
    }

double Ponto::getY(){
    return y;
    }

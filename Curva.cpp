#include "Curva.h"

Curva::Curva() {
}

Curva::Curva(const Curva& orig) {
}

Curva::~Curva() {
}

Curva::Curva(double * x , double * y,std::string n, int s,bool t) {
    
    nome = n;
    size = s;
    estado = t;
    
    for(int i = 0; i<size;i++){
        ponto[i] = Ponto(x[i],y[i]);
    }
    
}


double Curva::getX(int i){
    return ponto[i].getX();
}

double Curva::getY(int i){
    return ponto[i].getY();
}

double Curva::getU(int i){
    return ponto[i].getU();
}

double Curva::getV(int i){
    return ponto[i].getV();
}	 	  	     	  	      	     	 	    	        	 	

int Curva::getSize(){
    return size;
}

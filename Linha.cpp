

#include "Linha.h"

Linha::Linha() {
}

Linha::Linha(const Linha& orig) {
}

Linha::~Linha() {
}

Linha::Linha(double a, double b, double c , double d){

    tipo = 1;
    Coordenada b1(a,b,0);
    Coordenada b2(c,d,0);
    addCoordenada(b1);
    addCoordenada(b2);
    
}
Linha::Linha(double a, double b, double c , double d,std::string n){
    
    nome = n;
    tipo = 1;
    Coordenada b1(a,b,0);
    Coordenada b2(c,d,0);
    addCoordenada(b1);
    addCoordenada(b2);

}

void Linha::setP(double x , double y, bool p){
   Coordenada c(x,y,0);
    if(p){
        m_coords[0] =  c;
    }
    else{
        m_coords[1] = c;
    }
}	 	  	     	  	      	     	 	    	        	 	












